#include "socketsession.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include "../protocols/protocolvalidator.h"
// #include "../../common/utils.h"

SocketSession::SocketSession(QSslSocket* socket, AuthManager* authManager, QObject *parent)
    : QObject(parent)
{
    m_socket = socket;
    m_socket->setParent(this);
    connect(m_socket, &QSslSocket::disconnected,
            this, &SocketSession::onSocketDisconnected);
    connect(m_socket, &QSslSocket::destroyed,
            this, &SocketSession::onSocketDestroyed);
    connect(m_socket, &QSslSocket::readyRead,
            this, &SocketSession::onReadyRead);
    connect(m_socket, &QSslSocket::errorOccurred,
            this, &SocketSession::onErrorOccurred);

    m_timer.setInterval(SESSION_TIMEOUT_MS);
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, [this](){
        qDebug() << "Время жизни сокет-сессии истекло";
        closeSession();
    });
    m_timer.start();

    m_absTimer.setInterval(SESSION_MAX_LIFETIME_MS);
    m_absTimer.setSingleShot(true);
    connect(&m_absTimer, &QTimer::timeout, this, [this](){
        qDebug() << "Абсолютное время жизни сокет-сессии истекло";
        closeSession();
    });
    m_absTimer.start();

    qDebug() << "Новая сокет-сессия создана";
}

//==================================================//

void SocketSession::sendData(const QByteArray &msg)
{
    QJsonDocument doc;
    if (ProtocolValidator::validateMessage(msg, doc))
    {
        quint32 msgSize = msg.size();
        QByteArray packet;
        packet.resize(sizeof(quint32) + msgSize);
        qToBigEndian(msgSize, packet.data());
        memcpy(packet.data() + 4, msg.constData(), msgSize);
        m_socket->write(packet);
        m_socket->flush();

        extendLifetime();
    }
    else
        qDebug() << "Некорректный JSON(отпр):" << msg;
}

void SocketSession::onReadyRead()
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
            processIncomingMessage(msg);
        }
    }
}

void SocketSession::processIncomingMessage(const QByteArray &msg) //TODO: IMPELEMENT
{
    QJsonDocument doc;
    if (ProtocolValidator::validateMessage(msg, doc))
    {
        qDebug() << "Received: " << doc.toJson();
        extendLifetime();
    }
}

//==================================================//

void SocketSession::extendLifetime()
{
    m_timer.stop();
    m_timer.start();
}

void SocketSession::closeSession()
{
    if (!m_isClosed){
        qDebug() << "CloseSession";
        emit closed(this);
        m_isClosed = true;
    }
}

void SocketSession::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Ошибка в сокет-сессии:"
             << socketError;
    closeSession();
}

void SocketSession::onSocketDisconnected()
{
    qDebug() << "Сокет отключён";
    closeSession();
}

void SocketSession::onSocketDestroyed()
{
    qDebug() << "Сокет-сессия уничтожена(слот)";
    closeSession();
}
