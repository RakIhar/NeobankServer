# TODO 
ExchangeData - проверить

Устранить 1.00 как "Exchange rate unavailable" в checkExchangeRateAccessibility
И там же проверить API

ExchangeRates добавить проверки валют и проверить код

CreateCredit тоже вынести в transactionsservice

Куча мелочей с переводами и строками

Добавить транзакции в бд

В клиенте для транзакций улучшить таблицу, она неполная и кривая

Клиент не умеет обрабатывать Credit.Create

Клиент может выбрать замороженный счёт для попытки кредита (запретить и на уровне UI)

Сервер отправляет такие балансы: "1e+05", "18890", "0"

Возможно балансы перенаправлять в центах (qint64), а не строках 

transaction.before для идентичных валют:
"amount": "10721", 
"comission": "комиссия 1% : 1.59", 
"exchane_rate": "1", 
Неправильный amount, comission дублируется

transaction.before для различных валют(EUR->PLN):
"amount": "39400",
"comission": "комиссия 1% : 1.50",
"exchane_rate": "4",

(PLN->EUR)
Sent JSON:
 {
    "amount": "400.00",
    "from": "40",
    "session_id": "677fe2b4-bc9c-4e33-9a86-0e490f86f8b1",
    "to": "41",
    "token": "n6fYl7lC3Tpiabf3D5xtYbWEgyML6cXAi9F46k04zdZSYQwtJklGM7E0e8Q7mxOv",
    "type": "transaction.before"
} 
Received JSON:
 {
    "transaction": {
        "account": {
            "account_id": 41,
            "balance": "9900",
            "created_at": "2025-12-22T13:28:13Z",
            "currency": "EUR",
            "iban": "BY53NEOB17664100938196192583",
            "status": "active",
            "updated_at": "2025-12-22T22:43:38Z",
            "user_id": 1
        },
        "amount": "0",
        "comission": "комиссия 1% : 4.50",
        "error": "",
        "exchane_rate": "0",
        "result": true
    },
    "type": "transaction.before"
}


Received JSON:
 {
    "amount": "10.00",
    "description": "",
    "from": 41,
    "session_id": "ce46f4c4-3d69-4219-a26f-d48dc2ef7fb5",
    "to": 40,
    "token": "ousGtJ7ZzgGWNoJzKUkw8j725sqYweJPo6PESPYq5ZC7gYGd6hQE30tr26dCVVm1",
    "type": "transaction.create"
}

"[Logger middleware] enter"
"[Exception middleware] enter"
"[Authentification middleware] enter"
[Authentification] Session validated: "{ce46f4c4-3d69-4219-a26f-d48dc2ef7fb5}"
"[Session middleware] enter"
"[Router middleware] enter"
"[Authorization middleware] enter"
[Authorization] User authenticated, user_id: 1
"[EndpointInvoker middleware] enter"
"[TransactionCreate endpoint] enter"
"[TransactionCreate endpoint] exit"
Hello, world!
"[EndpointInvoker middleware] exit"
"[Authorization middleware] exit"
"[Router middleware] exit"
"[Session middleware] exit"
"[Authentification middleware] exit"
"[Exception middleware] exit"
"[Logger middleware] exit"
Sent JSON:
 {
    "result": true,
    "transaction": {
        "account_id": 40,
        "amount": "37.6",
        "counterparty_account_id": 41,
        "created_at": "2025-12-23T07:38:02Z",
        "currency": "PLN",
        "description": "",
        "metadata": {
            "comission": "1%",
            "exchane_rate": 4
        },
        "status": "done",
        "transaction_id": 72,
        "type": "credit"
    },
    "type": "transaction.create"
}



# Старое
Комиссия в BeforeTransaction возвращается как строка, в которой будет написан текст о комиссии (например "комиссия 3%")
Курс как и комиссия строкой
Добавляется к этому итоговый amount, который будет учитывать комиссию и курс - идентично и в BeforeTransaction и в самой транзакции

BeforeTransaction несогласовано на сервере и клиенте
BeforeTransaction несогласовано с транзакциями
Всё, что связано с транзакциями видимо вынести в специальный сервис
Добавить переводы между разными валютами - в принципе улучшить переводы, потому что они сильно костыльные

Если аккаунт удален/заморожен/системный с ним взаимодействовать нельзя - нельзя переводить и т.д.

Общий баланс в клиенте не работает - улучшить общий баланс:
1) первая опция, общий баланс для каждой валюты, типо
RUB 0, BYN 12334, EUR 12, для каждой валюты
2) вторая опция, общий баланс с переводами в конкретную валюты
RUB 2143254 (с аккаунтов со всех валют)

В клиенте можно добавить просмотр курсов валют на DashboardPage: выбор валют, кол-во из, курс и результат

В пагинации можно добавить поле count - чтобы знать кол-во страниц
Сортировать транзакции по дате - сервер присылает уже отсортированные

Выводить транзакции в таблицу или что-то такое, потому что строки очень плохо смотрятся
Выводить аккаунты в таблицу

Второстепенно:
Улучшить транспортный уровень: user_agent и ip_address, безопасность
разграничить auth_session и logic_session
Сделать многопоточность
Улучшить авторизацию и проверку ролей в ней
Исправить nullable в моделях

