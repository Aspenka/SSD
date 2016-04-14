#ifndef TIMER_H
#define TIMER_H

#include <QTimer>
#include <QObject>
#include <QPair>
#include <QString>
#include <QObject>
#include <mutex>
#include "CronParser.h"

//#define taskPair_t        QPair <QString, int>

class Timer : public QObject
{
    Q_OBJECT

public:
    explicit Timer(QObject *parent = 0); //конструктор
    Timer ( Timer const & obj, QObject *parent = 0 );
    ~Timer();   //деструктор

    void setSingleShot(bool singleShot);    //метод станавливает переменную singShot в нужное состояние
    Timer &     operator = (Timer const & obj);

private:
    CronParser parser;      //обработчик cron-выражений
    QString cronJob;        //cron-выражение
    int taskIndex;          //имя таска
    int timerStart = 0;     //время срабатывания таймера
    bool singShot = false;  //синглшот
    time_t nextExec = -1;   //время следующего срабатывания
    std::mutex mtxNextExec; //мьтекс для корректного срабатывания таймера

    void timerEvent(QTimerEvent *event) override;    //обработка событий таймера
    time_t calcDiffTime();  //подсчет разницы времени

signals:
    void timeout(int index);  //сигнал о том, что время таймера истекло

public slots:
    void start(QString cron, int index);        //запуск таймера
    void stop();                        //остановка таймера
};

#endif // TIMER_H
