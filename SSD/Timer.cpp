#include "Timer.h"
#include <assert.h>
#include <limits>
#include <iostream>
#include <stdio.h>
#include <QTimerEvent>
#include <QDateTime>
#include <QDebug>
#include <stdlib.h>

//конструктор
Timer::Timer( QObject *parent) : QObject(parent)
{
    connect(&parser, SIGNAL(isSingle(bool)), this, SLOT(setSingleShot(bool)));
}

Timer::Timer(const Timer &obj, QObject *parent) : QObject(parent)
{
    parser = obj.parser;
    cronJob = obj.cronJob;
    taskIndex = obj.taskIndex;
    timerStart = obj.timerStart;
    singShot = obj.singShot;
    nextExec = obj.nextExec;
}

//метод устанавливает синглшот
void Timer::setSingleShot(bool singleShot)
{
    singShot = singleShot;
}

//метод запускает таймер
void Timer::start(QString cron, int index)
{
    cronJob = cron;
    taskIndex = index;
    std::lock_guard<std::mutex> lock(mtxNextExec);
    nextExec = parser.getDateTime(cronJob).toTime_t();
    timerStart = startTimer(calcDiffTime());
    assert(timerStart);
}

//меотд останавливает таймер
void Timer::stop()
{
    std::lock_guard<std::mutex> lock(mtxNextExec);
    nextExec = -1;
    killTimer(timerStart);
}

//метод обрабатывает события таймера
void Timer::timerEvent(QTimerEvent *event)
{
    if(event->timerId() != timerStart)
        return;
    std::lock_guard<std::mutex> lock(mtxNextExec);
    killTimer(timerStart);
    timerStart = 0;
    if(nextExec > QDateTime::currentDateTime().toTime_t())
    {
        timerStart = startTimer(calcDiffTime());
        assert(timerStart);
    }
    else
    {
        emit timeout(taskIndex);
        if(!singShot)
        {
            nextExec = parser.getDateTime(cronJob).toTime_t();
            if(calcDiffTime() == 0)parser.setCall(true);
            timerStart = startTimer(calcDiffTime());
        }
        else
        {
            emit done(taskIndex);
        }
    }
}

//метод вычисляет время ожидания следующего вызова функции
time_t Timer::calcDiffTime()
{
    time_t t = QDateTime::currentDateTime().msecsTo(QDateTime::fromTime_t(0).addSecs(nextExec));
    if(t < 0)
        return 0;
    if(t > INT_MAX)
        return INT_MAX;
    return t;
}

//--
Timer & Timer::operator = ( Timer const & obj )
{
    parser = obj.parser;
    cronJob = obj.cronJob;
    taskIndex = obj.taskIndex;
    timerStart = obj.timerStart;
    singShot = obj.singShot;
    nextExec = obj.nextExec;
    return *this;
}


//деструктор
Timer::~Timer()
{

}

