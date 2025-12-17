#include "authentification.h"
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QDebug>

using namespace Services;

QString Authentification::hashPassword(const QString& password)
{
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(password.toUtf8());
    return hash.result().toHex();
}

QString Authentification::generateToken()
{
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    QString token;
    token.reserve(64);
    
    QRandomGenerator* rng = QRandomGenerator::global();
    for (int i = 0; i < 64; ++i) {
        token.append(chars.at(rng->bounded(chars.length())));
    }
    
    return token;
}

bool Authentification::verifyPassword(const QString& password, const QString& passwordHash)
{
    QString hashed = hashPassword(password);
    return hashed == passwordHash;
}

std::optional<Models::AuthSession> Authentification::authenticate(const QString& username, const QString& password,
                                                          const QString& ipAddress, const QString& userAgent)
{
    std::optional<Models::User> userOpt = m_userRepo->findByUsername(username);
    if (!userOpt.has_value()) {
        qDebug() << "[Authentification] User not found:" << username;
        return std::nullopt;
    }
    
    Models::User user = userOpt.value();
    
    if (user.status != "active") {
        qDebug() << "[Authentification] User is not active:" << username;
        return std::nullopt;
    }
    
    if (!verifyPassword(password, user.passwordHash)) {
        qDebug() << "[Authentification] Invalid password for user:" << username;
        return std::nullopt;
    }
    
    Models::AuthSession session;
    session.id = QUuid::createUuid();
    session.user_id = user.id;
    session.state = Models::AuthSessionState::Established;
    session.token = generateToken();
    session.ip_address = ipAddress;
    session.user_agent = userAgent;
    session.created_at = QDateTime::currentDateTimeUtc();
    session.expires_at = session.created_at.addDays(30);
    
    std::optional<Models::AuthSession> savedSession = m_authSessionRepo->upsert(session);
    if (!savedSession.has_value()) {
        qDebug() << "[Authentification] Failed to save session";
        return std::nullopt;
    }
    
    qDebug() << "[Authentification] User authenticated:" << username << "Session:" << session.id.toString();
    return savedSession;
}

std::optional<Models::AuthSession> Authentification::validateSession(const QUuid& sessionId)
{
    auto sessionOpt = m_authSessionRepo->findById(sessionId);
    if (!sessionOpt.has_value()) {
        return std::nullopt;
    }
    
    Models::AuthSession session = sessionOpt.value();
    
    if (session.state != Models::AuthSessionState::Established) {
        qDebug() << "[Authentification] Session is not established:" << sessionId.toString();
        return std::nullopt;
    }
    
    if (session.expires_at < QDateTime::currentDateTimeUtc()) {
        qDebug() << "[Authentification] Session expired:" << sessionId.toString();
        m_authSessionRepo->updateState(sessionId, Models::AuthSessionState::Expired);
        return std::nullopt;
    }
    
    return session;
}

std::optional<Models::AuthSession> Authentification::validateSessionByToken(const QString& token)
{
    auto sessionOpt = m_authSessionRepo->findByToken(token);
    if (!sessionOpt.has_value()) {
        return std::nullopt;
    }
    
    Models::AuthSession session = sessionOpt.value();
    
    if (session.state != Models::AuthSessionState::Established) {
        qDebug() << "[Authentification] Session is not established for token";
        return std::nullopt;
    }
    
    if (session.expires_at < QDateTime::currentDateTimeUtc()) {
        qDebug() << "[Authentification] Session expired for token";
        m_authSessionRepo->updateState(session.id, Models::AuthSessionState::Expired);
        return std::nullopt;
    }
    
    return session;
}

bool Authentification::logout(const QUuid& sessionId)
{
    bool updated = m_authSessionRepo->updateState(sessionId, Models::AuthSessionState::Revoked);
    if (updated) {
        qDebug() << "[Authentification] Session revoked:" << sessionId.toString();
    }
    return updated;
}

std::optional<Models::User> Authentification::registerUser(const QString &username,
                                                           const QString &password,
                                                           const QString &email,
                                                           const QString &phone)
{
    const QString normalizedUsername = username.trimmed();
    if (normalizedUsername.isEmpty() || password.isEmpty()) {
        qDebug() << "[Authentification] Username or password is empty";
        return std::nullopt;
    }

    if (m_userRepo->findByUsername(normalizedUsername).has_value()) {
        qDebug() << "[Authentification] Username already exists:" << normalizedUsername;
        return std::nullopt;
    }

    Models::User newUser;
    newUser.username = normalizedUsername;
    newUser.passwordHash = hashPassword(password);
    newUser.email = email.trimmed();
    newUser.phone = phone.trimmed();
    newUser.status = QStringLiteral("active");

    auto created = m_userRepo->create(newUser);
    if (!created.has_value()) {
        qDebug() << "[Authentification] Failed to create user:" << normalizedUsername;
        return std::nullopt;
    }

    qDebug() << "[Authentification] User registered:" << normalizedUsername;
    return created;
}

std::optional<Models::User> Authentification::getUserBySession(const QUuid& sessionId)
{
    std::optional<Models::AuthSession> sessionOpt = validateSession(sessionId);
    if (!sessionOpt.has_value()) {
        return std::nullopt;
    }
    
    return m_userRepo->findById(sessionOpt.value().user_id);
}
