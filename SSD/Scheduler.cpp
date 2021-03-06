#include <iostream>
#include "Scheduler.h"

//пустой конструктор
Scheduler::Scheduler(QObject *parent) : QObject(parent)
{

}

//конструктор, принимающий одно задание
Scheduler::Scheduler(Task &oneTsk, QObject *parent) : QObject(parent)
{
    append(oneTsk);
}

//конструктор, принимающий перечень заданий
Scheduler::Scheduler(QList<Task> &taskLst, QObject *parent) : QObject(parent)
{
    append(taskLst);
}

//добавить новое задание в список
void Scheduler::append(Task &oneTsk)
{
    taskList.append(oneTsk);
}

//добавить перечень новых заданий в список
void Scheduler::append(QList<Task> &taskLit)
{
    taskList.append(taskLit);
}

//удалить все задания, содержащие cron-выражение и имя таска, переданных в параметре
void Scheduler::remove(Task &oneTsk)
{
    for( int i = 0; i < taskList.size(); i++ )
    {
        if ( taskList[i] == oneTsk )
        {
            disconnect(timer[i], SIGNAL(timeout(int)), this, SLOT(slt_Reaction(int)));
            timer.remove(i, 1);
            remove ( i );
        }
    }
}

void Scheduler::remove(int index)
{
    qDebug() << "[Scheduler]:   timer " <<  index << " Stopped";
    disconnect(timer[index], SIGNAL(timeout(int)), this, SLOT(slt_Reaction(int)));
    timer.remove(index, 1);
    taskList.removeAt(index);
}

//очистить вектор срабатываний
void Scheduler::clear()
{
    taskList.clear();
}

//метод запускает планировщик
void Scheduler::start()
{
    std::cout << "[Scheduller]: Starting...\n";
    for(int i=0; i<taskList.size(); i++)
    {
        Timer *t = new Timer();
        timer.append(t);
        connect (timer.at(i), SIGNAL(timeout(int)), this, SLOT(slt_Reaction(int)));
        connect (timer.at(i), SIGNAL(done(int)), this, SIGNAL(sigDone(int)));
        timer[i]->start(taskList[i].getCronjob(), i);
        qDebug() << "Tmer " << i << " have cron " << taskList[i].getCronjob();
    }
}

void Scheduler::stop()
{
    std::cout << "[Scheduller]: Scheduler stoped\n";
    for( int i = 0; i < timer.size(); i++)
    {
        timer[i]->stop();
        disconnect(timer[i], SIGNAL(timeout(int)), this, SLOT(slt_Reaction(int)));
    }
}

void Scheduler::restart()
{
    stop();
    start();
}

//метод вызывает процедуры с нужным именем по таймауту таймера
void Scheduler::slt_Reaction(int index)
{
    emit sig_callTask(index);
}

//деструктор
Scheduler::~Scheduler()
{

}

