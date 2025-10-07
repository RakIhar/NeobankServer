#include <QCoreApplication>
#include <QTimer>
#include <QDateTime>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName("NeobankServer");
    a.setOrganizationName("R.I.Inc.");
    a.setApplicationVersion("1.0");

    // 1. Настройка логирования
    // 2. Настройка конфигурации
    // 3. Подключение к БД (если есть)
    // 4. Создание и запуск основного объекта (например сервера)
    // 5. Настройка таймеров, потоков и сигналов ?
    // 6. Запуск цикла обработки событий ?

    // // Создаём таймер
    // QTimer timer;
    // // Соединяем сигнал таймера со слотом (лямбдой)
    // QObject::connect(&timer, &QTimer::timeout, []() {
    //     qDebug() << "Сообщение каждые 5 секунд:" << QDateTime::currentDateTime().toString("hh:mm:ss");
    // });
    // // Запускаем таймер — интервал в миллисекундах
    // timer.start(5000); // 5000 мс = 5 секунд


    return a.exec(); //обработка сообщений от операционной системы
}
