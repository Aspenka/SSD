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
TaskManager::TaskManager (QByteArray data, QObject *parent) : QObject ( parent )
{
    //parse(data);----------------
}

//--
void TaskManager::run()
{
    Task task;
    if(!task.isEmpty())
    {
        taskList.append(task.getTaskList());
    }
    setConfig();
    //parse ( );  //--

    try
    {
        if ( !taskList.empty() )
        {
            QObject::connect( &scheduler, SIGNAL(sigDone(int)), this, SLOT(slt_taskDone(int)));
            QObject::connect( &scheduler, SIGNAL(sig_callTask(int)), this, SLOT(slt_Run(int)) );
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
        qDebug() << "[TaskManager.run()]:   taskList is empty!";
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
    scheduler.remove(index);
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
            scheduler.remove(i);
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
    scheduler.append(taskList);
    scheduler.start();
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

/*======================================================
метод обрабатывает пакет, поступивший из ЦС

параметры метода:
    QByteArray data - пакет с данными
======================================================*/
void TaskManager::parse ( QByteArray data )
{

}

//заглушка
void TaskManager::parse()
{
    QSettings set( "tasks.ini", QSettings::IniFormat );
    int size = set.beginReadArray("tasks");
    try
    {
        if ( size <= limit )
        {
            for ( int i = 0; i < size; i++ )
            {
                set.setArrayIndex( i );
                Task task;
                task.setCronjob ( set.value("cronjob").toString() );
                //task.setDevAddress ( set.value("devAddr").toString() );
                //task.setDevType ( set.value("devType").toString() );
                //task.setLogin ( set.value("login").toString() );
                //task.setPassword ( set.value("password").toString() );
                //task.setTask ( set.value("task").toString() );
                //task.setPriority( set.value("priority").toInt() );
                QString temp = set.value("arguments").toString();
                QStringList list;
                if(temp != "")
                {
                    if(temp.contains(","))
                    {
                        list = temp.split(",");
                    }
                    else
                    {
                        list.append(temp);
                    }
                    //task.setArgums(list);
                }
                else
                {
                    //task.setArgums(list);
                }
                task.save();
                addTask( task );
            }
        }
        else
        {
            throw limit;
        }
    }
    catch (int)
    {
        qDebug() << "[TaskManager.parser()]:    Error! Amount of tasks exceeds the limit value";
    }
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
    qDebug() << "[" << QDateTime::currentDateTime().toString("dd/MM/yy hh:mm:ss")
             << "]: Call task " << index << endl;
}

//--
void TaskManager::slt_taskDone(int index)
{
    updateTask(index, DONE);
}

/*======================================================
деструктор класса
======================================================*/
TaskManager::~TaskManager ( )
{

}

