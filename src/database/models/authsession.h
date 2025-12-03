#ifndef AUTHSESSION_H
#define AUTHSESSION_H
#include <QUuid>
#include <QDateTime>

enum class AuthSessionState
{
    Pending,
    Established,
    Expired,
    Revoked
};

struct AuthSession
{
    QUuid id;
    qint64 user_id = 0;
    AuthSessionState state = AuthSessionState::Pending;
    QString token;
    QString ip_address;
    QString user_agent;
    QDateTime created_at;
    QDateTime expires_at;
};

#endif // AUTHSESSION_H

/*
neobank=# \d+ auth_sessions
                                                 Table "public.auth_sessions"
   Column   |           Type           | Collation | Nullable | Default | Storage  | Compression | Stats target | Description
------------+--------------------------+-----------+----------+---------+----------+-------------+--------------+-------------
 id         | uuid                     |           | not null |         | plain    |             |              |
 user_id    | integer                  |           |          |         | plain    |             |              |
 state      | character varying(32)    |           | not null |         | extended |             |              |
 token      | character varying(128)   |           |          |         | extended |             |              |
 ip_address | character varying(64)    |           |          |         | extended |             |              |
 user_agent | text                     |           |          |         | extended |             |              |
 created_at | timestamp with time zone |           |          | now()   | plain    |             |              |
 expires_at | timestamp with time zone |           |          |         | plain    |             |              |
Indexes:
    "auth_sessions_pkey" PRIMARY KEY, btree (id)
    "auth_sessions_token_key" UNIQUE CONSTRAINT, btree (token)
    "idx_auth_sessions_user_id" btree (user_id)
Foreign-key constraints:
    "auth_sessions_user_id_fkey" FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE SET NULL
Referenced by:
    TABLE "audit_log" CONSTRAINT "audit_log_session_id_fkey" FOREIGN KEY (session_id) REFERENCES auth_sessions(id) ON DELETE SET NULL
Access method: heap
*/
