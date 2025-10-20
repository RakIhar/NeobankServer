#include "accountrepository.h"

AccountRepository::AccountRepository(QSqlDatabase db) : m_db(db){}

bool AccountRepository::create(const Account &account)
{
    QSqlQuery q(m_db);
    q.prepare("INSERTS INTO accounts (user_id, balance, currency) VALUES (:user_id, :balance, :currency)");
    q.bindValue(":user_id", account.userId);
    q.bindValue(":balance", account.balance);
    q.bindValue(":currency", account.currency);
    return q.exec();
}

Account AccountRepository::getById(int id)
{
    QSqlQuery q(m_db);
    q.prepare("SELECT id, user_id, balance, currency FROM accounts WHERE id = :id");
    q.bindValue(":id", id);
    q.exec();

    Account acc;
    if (q.next()) {
        acc.id = q.value("id").toInt();
        acc.userId = q.value("user_id").toInt();
        acc.balance = q.value("balance").toDouble();
        acc.currency = q.value("currency").toString();
    }
    return acc;
}

bool AccountRepository::updateBalance(int id, double newBalance)
{
    QSqlQuery q(m_db);
    q.prepare("UPDATE accounts SET balance = :balance WHERE id = :id");
    q.bindValue(":balance", newBalance);
    q.bindValue(":id", id);
    return q.exec();
}
