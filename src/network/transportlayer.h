#ifndef TRANSPORTLAYER_H
#define TRANSPORTLAYER_H

#include <QSslServer>
#include <QTimer>
#include <QPointer>
class ConnectionManager;

inline size_t qHash(const QPointer<QSslSocket> &ptr, size_t seed = 0) noexcept
{
    return qHash(reinterpret_cast<quintptr>(ptr.data()), seed);
}

class TransportLayer : public QObject
{
    Q_OBJECT
public: //ИНТЕРФЕЙС СЕРВЕРА
    explicit TransportLayer(QObject *parent = nullptr);
    void start();
    void stop();
public slots: //ИНТЕРФЕЙС ОБМЕНА СООБЩЕНИЯМИ МЕЖДУ СЛОЯМИ
    void onSendMessage(const QUuid &connection, const QByteArray &rawData); //делать ли слотами
    void close(const QUuid &connection);                                  //
signals:
    void messageReceived(const QUuid &connection, const QByteArray &rawData);
    void closed(const QUuid &connection);

private slots: //ОБЕРТКА НАД QSslServer
    void onEncryptedReady();
    void cleanup();
    void onSslErrors(QSslSocket *socket, const QList<QSslError> &errors);
    void onStartedEncryptionHandshake(QSslSocket *socket);
private:
    ConnectionManager* m_sessionManager;
    QTimer m_cleanupTimer;
    QSslServer* m_sslServer;
    QSet<QPointer<QSslSocket>> m_activeSockets;
    void initializeServerConfig();

};

#endif // TRANSPORTLAYER_H
