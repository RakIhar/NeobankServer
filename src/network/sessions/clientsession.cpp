#include "clientsession.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include "../protocols/protocolvalidator.h"

ClientSession::ClientSession(QSslSocket* socket, AuthManager* authManager, QObject *parent)
    : QObject(parent), m_authManager(authManager)
{
    m_socket = socket;
    m_socket->setParent(this);
    connect(m_socket, &QSslSocket::disconnected,
            this, &ClientSession::onSocketDisconnected);
    connect(m_socket, &QSslSocket::destroyed,
            this, &ClientSession::onSocketDestroyed);
    connect(m_socket, &QSslSocket::readyRead,
            this, &ClientSession::onReadyRead);
    connect(m_socket, &QSslSocket::errorOccurred,
            this, &ClientSession::onErrorOccurred);

    constexpr int TEN_MINUTES = 600000; //TODO: improve or delete timer
    m_timer.setInterval(TEN_MINUTES);
    m_timer.setSingleShot(true);

    connect(&m_timer, &QTimer::timeout, this, [this](){
        this->m_socket->disconnectFromHost();
    });
    m_timer.start();
}

void ClientSession::sendData(const QByteArray &msg)
{
    quint32 msgSize = msg.size();
    QByteArray packet;
    packet.resize(sizeof(quint32) + msgSize);
    qToBigEndian(msgSize, packet.data());
    memcpy(packet.data() + 4, msg.constData(), msgSize);
    m_socket->write(packet);
    m_socket->flush();
}

void ClientSession::onReadyRead()
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

void ClientSession::extendLifetime() //TODO: improve session lifetime
{
    constexpr int TEN_MINUTES = 600000;
    m_timer.setInterval(TEN_MINUTES);
}

void ClientSession::processIncomingMessage(const QByteArray &msg) //TODO: IMPELEMENT
{
    QJsonDocument doc;
    if (ProtocolValidator::validateMessage(msg, doc))
    {
        qDebug() << "Received: " << doc.toJson();
    }
}

void ClientSession::onErrorOccurred(QAbstractSocket::SocketError socketError) //TODO: implement
{
    emit closed(this);
}

void ClientSession::onSocketDisconnected() //TODO: implement
{
    emit closed(this);
}

void ClientSession::onSocketDestroyed() //TODO: implement
{
    emit closed(this);
}
