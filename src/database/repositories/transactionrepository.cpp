#include "transactionrepository.h"

TransactionRepository::TransactionRepository(QSqlDatabase db) : m_db(db){}

bool TransactionRepository::addTransaction(const Transaction &t)
{
    QSqlQuery q(m_db);
    q.prepare("INSERT INTO transactions (account_id, amount, type, timestamp) "
              "VALUES (:account_id, :amount, :type, :timestamp)");
    q.bindValue(":account_id", t.accountId);
    q.bindValue(":amount", t.amount);
    q.bindValue(":type", t.type);
    q.bindValue(":timestamp", t.timestamp);
    return q.exec();
}

QList<Transaction> TransactionRepository::getByAccount(int accountId)
{
    QList<Transaction> list;
    QSqlQuery q(m_db);
    q.prepare("SELECT id, account_id, amount, type, timestamp "
              "FROM transactions WHERE account_id = :account_id");
    q.bindValue(":account_id", accountId);
    q.exec();

    while (q.next()) {
        Transaction t;
        t.id = q.value("id").toInt();
        t.accountId = q.value("account_id").toInt();
        t.amount = q.value("amount").toDouble();
        t.type = q.value("type").toString();
        t.timestamp = q.value("timestamp").toDateTime();
        list.append(t);
    }
    return list;
}
