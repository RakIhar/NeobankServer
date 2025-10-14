#include "clientsession.h"

ClientSession::ClientSession(QObject *parent, QPointer<QSslSocket> socket)
    : QObject{parent}, m_socket(socket)
{
    constexpr int TEN_MINUTES = 600000;
    m_timer.setInterval(TEN_MINUTES);
    m_timer.setSingleShot(true);

    connect(&m_timer, &QTimer::timeout, this, [this](){
        this->m_socket->disconnectFromHost();
        emit expired(this);
    });
    m_timer.start();
}

void ClientSession::onErrorOccurred(QAbstractSocket::SocketError socketError)
{

}

void ClientSession::onReadyRead()
{

}

void ClientSession::onDisconnected()
{

}
