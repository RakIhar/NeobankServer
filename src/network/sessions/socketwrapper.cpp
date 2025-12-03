#include "socketwrapper.h"

SocketWrapper::SocketWrapper(const QUuid &sessionId,
                             QSslSocket *socket,
                             QObject *parent)
    : QObject(parent),
      m_sessionId(sessionId),
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

    qDebug() << "Новая сокет-сессия создана" << m_sessionId;
}

void SocketWrapper::sendData(const QByteArray &msg)
{
    quint32 msgSize = msg.size();
    QByteArray packet;
    packet.resize(sizeof(quint32) + msgSize);
    qToBigEndian(msgSize, packet.data());
    memcpy(packet.data() + 4, msg.constData(), msgSize);
    m_socket->write(packet);
    m_socket->flush();
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
            emit messageReceived(m_sessionId, msg);
        }
    }
}

void SocketWrapper::closeSession()
{
    if (!m_isClosed){
        qDebug() << "CloseSession";
        m_isClosed = true;
        emit closed(m_sessionId);
    }
}

void SocketWrapper::terminate()
{
    if (!m_isClosed)
    {
        qDebug() << "Принудительное завершение сессии" << m_sessionId;
        closeSession();
    }
}
