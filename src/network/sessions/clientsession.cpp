#include "clientsession.h"

ClientSession::ClientSession(QSslSocket* socket, AuthManager* authManager, QObject *parent) : QObject(parent), m_authManager(authManager)
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

    constexpr int TEN_MINUTES = 600000;
    m_timer.setInterval(TEN_MINUTES);
    m_timer.setSingleShot(true);

    connect(&m_timer, &QTimer::timeout, this, [this](){
        this->m_socket->disconnectFromHost();
    });
    m_timer.start();

    // DEBUG
    /*
    connect(m_socket, &QSslSocket::disconnected,
            [this]{qDebug() << "m_socket disconnected";});
    connect(m_socket, &QSslSocket::destroyed,
            [this]{qDebug() << "m_socket destroyed";});
    connect(m_socket, &QSslSocket::aboutToClose,
            [this]{qDebug() << "m_socket aboutToClose";});
    connect(m_socket, &QSslSocket::readyRead,
            [this]{qDebug() << "m_socket readyRead";});
    // connect(m_socket, &ClientSession::authenticated,
            // [this](){qDebug() << "m_socket authenticated";});
    // connect(m_socket, &ClientSession::closed,
            // [this](){qDebug() << "m_socket closed";});
    connect(m_socket, &ClientSession::destroyed,
            [this](){qDebug() << "session destroyed";});
    */
}

void ClientSession::sendData(const QByteArray &data)
{
    qDebug() << "ClientSession::sendData";  //DEBUG
    qDebug() << data;                       //DEBUG

    if (!m_socket) return;

    m_socket->write(data);
    m_socket->flush(); // TLS сразу шифрует и отправляет

    emit authenticated("123123", this);
}

void ClientSession::processIncomingData(const QByteArray &data)
{
    if (m_authContext.state == AuthState::Completed)
    {
        // qDebug() << "ClientSession::processIncomingData";   //DEBUG
        qDebug() << data;                                   //DEBUG
        sendData("Server Hello");                           //DEBUG
    }
    else
    {
        m_authManager->processStep(m_authContext, data);
        if (m_authContext.state == AuthState::Completed)
        {
            qDebug() << "authenticated";
            QByteArray sessionId;
            emit authenticated(sessionId, this);
        }
        if (m_authContext.state == AuthState::Failed)
        {
            qDebug() << "failed";
        }
        if (m_authContext.state == AuthState::InProgress)
        {
            qDebug() << "in progress";
        }
    }
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
