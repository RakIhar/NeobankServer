#include "clientsession.h"

ClientSession::ClientSession(QSslSocket *socket, QObject *parent) : QObject{parent}
{
    m_socket = socket;
    m_socket->setParent(this);
    connect(socket, &QSslSocket::disconnected,
            this, &ClientSession::onSocketDisconnected);
    connect(socket, &QSslSocket::destroyed,
            this, &ClientSession::onSocketDestroyed);
    connect(m_socket, &QSslSocket::readyRead,
            this, &ClientSession::onReadyRead);
    connect(m_socket, &QSslSocket::errorOccurred,
            this, &ClientSession::onErrorOccurred);

    constexpr int TEN_MINUTES = 600000;
    m_timer.setInterval(TEN_MINUTES);
    m_timer.setSingleShot(true);

    connect(&m_timer, &QTimer::timeout, this, [this](){
        this->m_socket->disconnectFromHost();
    });
    m_timer.start();
}

void ClientSession::sendData(const QByteArray &data)
{
    if (!m_socket) return;

    m_socket->write(data);
    m_socket->flush(); // TLS сразу шифрует и отправляет
}

void ClientSession::processIncomingData(const QByteArray &data)
{

}

void ClientSession::extendLifetime()
{
    constexpr int TEN_MINUTES = 600000;
    m_timer.setInterval(TEN_MINUTES);
}

void ClientSession::onReadyRead()
{
    if (!m_socket) return;

    while (m_socket->bytesAvailable() > 0) {
        QByteArray data = m_socket->readAll();
        processIncomingData(data);
    }
}

//===================================================================//

void ClientSession::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    emit closed(this);
}

void ClientSession::onSocketDisconnected()
{
    emit closed(this);
}

void ClientSession::onSocketDestroyed()
{
    emit closed(this);
}
