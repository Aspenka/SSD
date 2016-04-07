#include <iostream>
#include "Scheduler.h"

//пустой конструктор
Scheduler::Scheduler(QObject *parent) : QObject(parent)
{

}

//конструктор, принимающий одно задание
Scheduler::Scheduler(taskPair_t oneTask, QObject *parent) : QObject(parent)
{
    append(oneTask);
}

//конструктор, принимающий перечень заданий
Scheduler::Scheduler(taskVector_t taskVector, QObject *parent) : QObject(parent)
{
    append(taskVector);
}

//добавить новое задание в список
void Scheduler::append(taskPair_t oneTask)
{
    taskVect.append(oneTask);
}

//добавить перечень новых заданий в список
void Scheduler::append(taskVector_t taskVector)
{
    taskVect = taskVector;
}

//удалить все задания, содержащие cron-выражение и имя таска, переданных в параметре
void Scheduler::remove(taskPair_t oneTask)
{
    taskVect.removeOne(oneTask);
}

void Scheduler::remove(int index)
{
    taskVect.remove(index,1);
}

//очистить вектор срабатываний
void Scheduler::clear()
{
    taskVect.clear();
}

//метод запускает планировщик
void Scheduler::start()
{
    std::cout << "[Scheduller]: Starting...\n";
    for(int i=0; i<taskVect.size(); i++)
    {
        timer = new Timer();
        connect(timer, SIGNAL(timeout(QPair<QString,int>)), this, SLOT(slotReaction(QPair<QString,int>)));
        timer->start(taskVect.at(i));
    }
}

//метод вызывает процедуры с нужным именем по таймауту таймера
void Scheduler::slotReaction(taskPair_t task)
{
    //std::cout << "[" << QDateTime::currentDateTime().toString("dd/MM/yy hh:mm:ss").toStdString();
    //std::cout << "]: Call " << task.second.toStdString() << std::endl;
    emit callTask(task.second);
}

//деструктор
Scheduler::~Scheduler()
{

}

