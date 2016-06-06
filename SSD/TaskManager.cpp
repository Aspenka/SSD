#include <QSettings>
#include <QDebug>
#include <QDateTime>
#include <QSettings>
#include "TaskManager.h"

/*======================================================
конструктор класса
======================================================*/
TaskManager::TaskManager ( QObject *parent ) : QObject ( parent )
{

}

//--
void TaskManager::run()
{
    setConfig();
    Task task;
    int size = task.getTaskList().size();
    try
    {
        if(size <= limit)
        {
            if(!task.isEmpty())
            {
                taskList.append(task.getTaskList());
            }
        }
        else
        {
            throw size;
        }
    }
    catch(int)
    {
        qDebug() << "[!][TASK MANAGER]:\tError! Task list stack overflow!";
    }

    try
    {
        if ( !taskList.empty() )
        {
            for ( int i = 0; i < taskList.size(); i++ )
            {
                QObject::connect( &taskList[i], SIGNAL(sig_done(Task)), this, SLOT(slt_OnDone(Task)) );
            }

            handleTask();
        }
        else
        {
            throw taskList;
        }
    }
    catch ( QList <Task> )
    {
        qDebug() << "[!][TASK MANAGER]:\ttaskList is empty!";
    }
}

//--
void TaskManager::setConfig()
{
    QSettings set ("config.cfg", QSettings::IniFormat);
    set.beginGroup("task_param");
    limit = set.value("limit").toInt();
    set.endGroup();
}

/*======================================================
метод добавляет в список новую задачу

параметры метода:
    Task task - объект задача;
======================================================*/
void TaskManager::addTask (Task & task)
{
    taskList.append(task);
}

/*======================================================
метод удаляет задачу из списка по ее порядковому номеру

параметры метода:
    int index - порядковый номер задачи в списке
======================================================*/
void TaskManager::removeTask ( int index )
{
    taskList[index].remove();    
    qDebug() << "[TASK MANAGER]:\ttimer " <<  index << " Stopped";
    disconnect(timer[index], SIGNAL(timeout(int)), this, SLOT(slt_Reaction(int)));
    timer.remove(index, 1);
    taskList.removeAt(index);
}

/*======================================================
метод удаляет задачу из списка по ее содержимому

параметры метода:
    Task task - объект задача
======================================================*/
void TaskManager::removeTask ( Task task )
{
    for ( int i = 0; i < taskList.size(); i++ )
    {
        if ( taskList[i] == task )
        {
            taskList[i].remove();
            disconnect(timer[i], SIGNAL(timeout(int)), this, SLOT(slt_Reaction(int)));
            qDebug() << "[TASK MANAGER]:\ttimer " <<  i << " Stopped";
            timer.remove(i, 1);
            taskList.removeAt(i);
            break;
        }
    }
}

/*======================================================
метод очищает список задач
======================================================*/
void TaskManager::clear ( )
{
    taskList.clear();
}

/*======================================================
метод запускает процесс обработки задачи

параметры метода:
    Task task - объект задача
======================================================*/
void TaskManager::handleTask ()
{
    for ( int i = 0; i < taskList.size(); i++ )
    {
        taskList[i].setStatus(PROCESSING);
        updateTask(i, PROCESSING);
    }
    startTimer();
}

/*======================================================
метод изменяет данные о задаче

параметры метода:
    int index - порядковый номер задачи в списке;
    int status - статус задачи, параметр является
                 необязательным
======================================================*/
void TaskManager::updateTask ( int index, int status)
{
    taskList[index].edit(status);
}

//заглушка
void TaskManager::print()
{
    for( int i=0; i<taskList.size(); i++ )
    {
        taskList[i].print();
        qDebug() << "-----------------------------";
    }
}

//--
void TaskManager::slt_OnDone(Task task)
{
    removeTask( task );
}

/*======================================================
метод-слот, запускающий  задачу на выполнение

параметры метода:
    int index - порядковый номер задачи в списке
======================================================*/
void TaskManager::slt_Run (int index)
{
    qDebug() << "[TASK MANAGER]" << "[" << QDateTime::currentDateTime().toString("dd/MM/yy hh:mm:ss")
             << "]:\tCall device " << taskList[index].getDeviceUid() << endl;
    emit sig_callTask(taskList[index].getDeviceUid());
}

//--
void TaskManager::slt_taskDone(int index)
{
    updateTask(index, DONE);
}

//--
void TaskManager::startTimer()
{
    qDebug() << "[TASK MANAGER]:\tStarting scheduler...\n";
    for(int i=0; i<taskList.size(); i++)
    {
        Timer *t = new Timer();
        timer.append(t);
        connect (timer.at(i), SIGNAL(timeout(int)), this, SLOT(slt_Run(int)));
        connect (timer.at(i), SIGNAL(done(int)), this, SLOT(slt_taskDone(int)));
        timer[i]->start(taskList[i].getCronjob(), i);
        qDebug() << "[TASK MANAGER]:\tTmer " << i << " have cron " << taskList[i].getCronjob();
    }
}

void TaskManager::stop()
{
    qDebug() << "[TASK MANAGER]:\tScheduler stopped\n";
    for( int i = 0; i < timer.size(); i++)
    {
        timer[i]->stop();
        disconnect(timer[i], SIGNAL(timeout(int)), this, SLOT(slt_Run(int)));
    }
}

/*======================================================
деструктор класса
======================================================*/
TaskManager::~TaskManager ( )
{

}

