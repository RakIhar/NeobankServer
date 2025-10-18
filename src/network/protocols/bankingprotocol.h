#ifndef BANKINGPROTOCOL_H
#define BANKINGPROTOCOL_H

//определение самого протокола
/*
Заголовок — тип сообщения, длина, версия.

Маркер сессии — ID клиента, токен, nonce.

Команду или код операции — login, ping, data, close и т.д.

Метаданные — кодировка, MIME-тип, приоритет.

Полезную нагрузку — сериализованные данные (JSON, CBOR, protobuf).

Контрольную сумму / подпись — для проверки целостности и подлинности.

Флаги управления — начало, конец пакета, необходимость ответа.
*/
class BankingProtocol
{
public:
    BankingProtocol();
};

#endif // BANKINGPROTOCOL_H
