#include "socketwrapper.h"

SocketWrapper::SocketWrapper(const QUuid &sessionId,
                             QSslSocket *socket,
                             QObject *parent)
    : QObject(parent),
    m_socket(socket)
{
    m_socket->setParent(this);

    connect(m_socket, &QSslSocket::disconnected, this, [this](){
        qDebug() << "Сокет отключён";
        closeSession();
    });

    connect(m_socket, &QSslSocket::destroyed, this, [this](){
        qDebug() << "Сокет-сессия уничтожена(слот)";
        closeSession();
    });

    connect(m_socket, &QSslSocket::errorOccurred, this, [this](QAbstractSocket::SocketError socketError) {
        qDebug() << "Ошибка в сокет-сессии:" << socketError;
        closeSession();
    });

    connect(m_socket, &QSslSocket::readyRead,
            this, &SocketWrapper::onReadyRead);

    qDebug() << "Новая сокет-сессия создана" << sessionId;


    m_info.id = sessionId;

    m_info.createdAt = QDateTime::currentDateTimeUtc();
    m_info.lastActivity = m_info.createdAt;
    m_info.peerAddress = socket->peerAddress().toString().toUtf8();
    m_info.peerPort = socket->peerPort();
}

void SocketWrapper::sendData(const QByteArray &rawData)
{
    quint32 msgSize = rawData.size();
    QByteArray packet;
    packet.resize(sizeof(quint32) + msgSize);
    qToBigEndian(msgSize, packet.data());
    memcpy(packet.data() + 4, rawData.constData(), msgSize);
    m_socket->write(packet);
    m_socket->flush();

    updateLastActivity();
}

void SocketWrapper::onReadyRead()
{
    m_buffer.append(m_socket->readAll());
    if (m_buffer.size() >= sizeof(quint32))
    {
        quint32 msgSize;
        memcpy(&msgSize, m_buffer.constData(), sizeof(quint32));
        msgSize = qFromBigEndian(msgSize);

        if (m_buffer.size() >= sizeof(quint32) + msgSize)
        {
            QByteArray msg = m_buffer.mid(sizeof(quint32), msgSize);
            m_buffer.remove(0, sizeof(quint32) + msgSize);
            emit messageReceived(m_info.id, msg);
        }
    }

    updateLastActivity();
}

void SocketWrapper::closeSession()
{
    if (!m_isClosed){
        qDebug() << "CloseSession";
        m_isClosed = true;
        emit closed(m_info.id);
    }
}

void SocketWrapper::updateLastActivity()
{
    m_info.lastActivity = QDateTime::currentDateTimeUtc();
}

void SocketWrapper::close()
{
    if (!m_isClosed)
    {
        qDebug() << "Принудительное завершение сессии" << m_info.id;
        closeSession();
    }
}
