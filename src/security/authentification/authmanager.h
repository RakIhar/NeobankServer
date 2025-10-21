#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QObject>
#include <QSslSocket>
#include <QString>

//Сделать фабрику

enum class AuthState {
    None,        // ещё не начинали
    InProgress,  // идёт процесс (может быть OTP)
    Completed,   // всё ок
    Failed       // ошибка
};

struct AuthContext {
    AuthState state = AuthState::None;
    QByteArray login; //или QString
    QByteArray passwordHash;
    QByteArray otp;
};

enum class AuthStatus {
    Success,   // авторизация завершена
    Pending,   // нужно продолжить (например, ждём OTP)
    Fail
};

class AuthManager : public QObject
{
    Q_OBJECT
public:
    static AuthManager* instance();

    AuthStatus processStep(AuthContext &ctx, const QByteArray &message);

private:
    explicit AuthManager(QObject *parent = nullptr);
    AuthManager(const AuthManager&) = delete;
    AuthManager& operator=(const AuthManager&) = delete;

    bool verifyCredentials(const QByteArray &login, const QByteArray &password);
    bool isOtpRequired(const QByteArray &login);
    bool verifyOtp(const QByteArray &login, const QByteArray &otp);
signals:
};

#endif // AUTHMANAGER_H


/*
    Веб-приложение позволяет пользователям создавать простые пароли.
    Веб-приложение не защищено от возможности перебора паролей (brute-force attacks).
    Веб-приложение само генерирует и распространяет пароли пользователям, однако не требует смены пароля после первого входа (т.е. текущий пароль где-то записан).
    Веб-приложение допускает передачу паролей по незащищенному HTTP-соединению, либо в строке URL.
    Веб-приложение не использует безопасные хэш-функции для хранения паролей пользователей.
    Веб-приложение не предоставляет пользователям возможность изменения пароля либо не нотифицирует пользователей об изменении их паролей.
    Веб-приложение использует уязвимую функцию восстановления пароля, которую можно использовать для получения несанкционированного доступа к другим учетным записям.
    Веб-приложение не требует повторной аутентификации пользователя для важных действий: смена пароля, изменения адреса доставки товаров и т. п.
    Веб-приложение создает session tokens таким образом, что они могут быть подобраны или предсказаны для других пользователей.
    Веб-приложение допускает передачу session tokens по незащищенному HTTP-соединению, либо в строке URL.
    Веб-приложение уязвимо для session fixation-атак (т. е. не заменяет session token при переходе анонимной сессии пользователя в аутентифицированную).
    Веб-приложение не устанавливает флаги HttpOnly и Secure для browser cookies, содержащих session tokens.
    Веб-приложение не уничтожает сессии пользователя после короткого периода неактивности либо не предоставляет функцию выхода из аутентифицированной сессии.
*/
