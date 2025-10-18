#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QObject>
#include "clientsession.h"
#include <QPointer>
#include <QSet>
#include <QTcpSocket>
#include <QSslSocket>
#include <QPointer>
#include "../../security/authentification/authmanager.h"

//Отвечает за жизненный цикл сессий.
//Создаёт ClientSession и хранит их.
//Подписывает сокет на read/write/error/disconnected в рамках конкретной сессии.

//Аутентификация:
//1 Аутентификация пользователя (покуда без двухфакторки)
//2 Создание SessionID
//3 Связывание SessionID + UserID в базе данных
//4 Передача SessionID клиенту
//5 Запись входа (опционально)

//При обращении к API:
//1 Проверить наличие и валидность(в бд) SessionID
//2 Проверить время бездействия (?)
//3 Одноразовые токены

//Завершение сессии:
//1 Логаут - кнопка выйти
//2 Таймаут
//3 Абсолютный таймаут
//4 Параллельный логин
//5 Принудительное завершение

//Сессия привязана к соединению(наверное)
/*

Идентификация — это заявление о том, кем вы являетесь. В зависимости от ситуации, это может быть имя, адрес электронной почты, номер учетной записи, итд.
Аутентификация — предоставление доказательств, что вы на самом деле есть тот, кем идентифицировались (от слова “authentic” — истинный, подлинный).
Авторизация — проверка, что вам разрешен доступ к запрашиваемому ресурсу.

Пользователь логинится — отправляет логин/пароль
→ POST /login с {"username": "...", "password": "..."}
Сервер проверяет их и создаёт сессию
→ Сохраняем в памяти или базе связь сессии с учеткой
Сервер присваивает сессии уникальный ID (обычно — случайная строка)
→ Генерируем session_id
Этот ID отправляется пользователю в cookie
При каждом следующем запросе браузер автоматически отправляет cookie
Сервер по ID находит сессию и “узнаёт” пользователя
→ сверяем сессию в базе и вытаскиваем учетку к которой привязана сессия

*/
inline size_t qHash(const QPointer<ClientSession> &ptr, size_t seed = 0) noexcept {
    return qHash(ptr.data(), seed);
}

class SessionManager : public QObject
{
    Q_OBJECT
public:
    explicit SessionManager(QObject *parent = nullptr);

    void createSession(QPointer<QSslSocket> socket);
    // void removeSession(ClientSession *session);
    // int activeCount() const;
private:
    QSet<QPointer<ClientSession>> m_sessions;
    QTimer m_cleanupTimer;
    AuthManager m_authManager;
private slots:
    void onSessionExpired(ClientSession *session);
    void cleanup();
signals:
};

#endif // SESSIONMANAGER_H
