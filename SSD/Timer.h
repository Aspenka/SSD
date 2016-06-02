#ifndef TIMER_H
#define TIMER_H

#include <QTimer>
#include <QObject>
#include <QPair>
#include <QString>
#include <QObject>
#include <mutex>
#include "CronParser.h"

/*=============================================================
 Класс Timer выполняет следующие функции:
 -обрабатывает cron-выражение задачи посредством объекта
  класса CronParser и получает время ближайшего выполнения
  задачи;
 -отслеживает время начала выполнения задачи

 Атрибуты класса:
 1) parser - обработчик cron-выражений;
 2) cronJob - cron-выражение;
 3) taskIndex - индекс задачи;
 4) timerStart - время срабатывания таймера;
 5) singShot - флаг синглшота задачи;
 6) nextExec - время следующего срабатывания;
 7) mtxNextExec - мьтекс для корректного срабатывания таймера;

 Методы класса:
 1) Timer() - конструктор по умолчанию;
 2) Timer (Timer const & obj) - конструктор копирования;
 3) ~Timer() - деструктор;
 4) Timer & operator = (Timer const & obj) - перегрузка
                                             операции
                                             присваивания;
 5) void timerEvent(QTimerEvent *event) - метод по обработке
                                          событий таймера;
 6) time_t calcDiffTime() - метод осуществляет подсчет
                            разницы времени;
 Сигналы класса:
 1) void timeout(int index) - сигнал о том, что время таймера
                              истекло;
 2) void done(int index) - сигнал о том, что задача выполнена

 Слоты класса:
 1) void start(QString cron, int index) - метод осуществляет
                                          запуск таймера;
 2) void stop() - метод осуществляет остановку таймера;
 3) void stop() - метод устанавливает переменную singShot в
                  нужное состояние.
 ============================================================*/

class Timer : public QObject
{
    Q_OBJECT

public:
    explicit Timer(QObject *parent = 0);
    Timer ( Timer const & obj, QObject *parent = 0 );
    ~Timer();


    Timer &     operator = (Timer const & obj);

private:
    CronParser parser;
    QString cronJob;
    int taskIndex;
    int timerStart = 0;
    bool singShot = false;
    time_t nextExec = -1;
    std::mutex mtxNextExec;

    void timerEvent(QTimerEvent *event) override;
    time_t calcDiffTime();

signals:
    void timeout(int index);
    void done(int index);

public slots:
    void start(QString cron, int index);
    void stop();
    void setSingleShot(bool singleShot);
};

#endif // TIMER_H
