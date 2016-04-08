#include <QSettings>
#include <QDebug>
#include <QDateTime>
#include "TaskManager.h"

/*======================================================
конструктор класса
======================================================*/
TaskManager::TaskManager ( QObject *parent ) : QObject ( parent )
{

}

//заглушка
TaskManager::TaskManager (QByteArray data, QObject *parent) : QObject ( parent )
{
    //parse(data);
    database = QSqlDatabase::addDatabase("QSQLITE", "dbConnection");
    database.setDatabaseName("base.db");
}

//заглушка
void TaskManager::run()
{
    database = QSqlDatabase::addDatabase("QSQLITE", "dbConnection");
    database.setDatabaseName("base.db");

    clearTaskTable();
    //checkTaskTable();------------------
    parse ( );  //--
    taskVector_t vector = toTaskVector();
    handleTask(vector);

    QObject::connect( &scheduler, SIGNAL(callTask(int)), this, SLOT(slt_Run(int)) );
    for ( int i = 0; i < taskList.size(); i++ )
    {
        QObject::connect( &taskList[i], SIGNAL(sig_done(Task)), this, SLOT(slt_OnDone(Task)) );
    }
}

/*======================================================
метод добавляет в список новую задачу

параметры метода:
    Task task - объект задача;
    int priority - приоритет задачи. Параметр является
                   необязательным.
======================================================*/
void TaskManager::addTask ( Task & task, int priority )
{
    if( priority != 0 )
    {
        task.setPriority( priority );
    }
    taskList.append(task);
    sortTasklist(taskList, 0, taskList.size()-1);
}

/*======================================================
метод упорядочивает задачи в соответствии с их
приоритетом
======================================================*/
void TaskManager::sortTasklist(QList<Task> & list, int left, int right)
{
    int i = left;
    int j = right;
    int p;
    Task temp ( database );
    p = list[(left + right )/2].getPriority();
    do
    {
        while ( list[i].getPriority() > p )
        {
            i++;
        }
        while ( list[j].getPriority() < p )
        {
            j--;
        }
        if ( i <= j )
        {
            temp = list[i];
            list[i] = list[j];
            list[j] = temp;
            i++;
            j--;
        }
    }
    while ( i <= j );
    if ( j > left )
    {
        sortTasklist ( list, left, j);
    }
    if ( i < right )
    {
        sortTasklist( list, i, right );
    }
}

/*======================================================
метод удаляет задачу из списка по ее порядковому номеру

параметры метода:
    int index - порядковый номер задачи в списке
======================================================*/
void TaskManager::removeTask ( int index )
{
    taskList[index].removeTask();
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
            taskList[i].removeTask();
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
void TaskManager::handleTask (QVector<QPair<QString, int> > vector )
{
    scheduler.append ( vector );
    scheduler.start();
    for ( int i = 0; i < taskList.size(); i++ )
    {
        taskList[i].setStatus(PROCESSING);
        updateTask(i, PROCESSING);
    }
}

/*======================================================
метод изменяет данные о задаче

параметры метода:
    int index - порядковый номер задачи в списке;
    int status - статус задачи, параметр является
                 необязательным;
    int priority - приоритет задачи, параметр является
                   необязательным
======================================================*/
void TaskManager::updateTask ( int index, int status, int priority )
{
    if ( status !=0 )
    {
        taskList[index].setStatus( status );
    }
    if ( priority != 0 )
    {
        taskList[index].setPriority( priority );
    }
    taskList[index].editTask(status, priority);
    sortTasklist(taskList, 0, taskList.size() - 1);
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
void TaskManager::parse ( )
{
    QSettings set( "tasks.ini", QSettings::IniFormat );
    int size = set.beginReadArray("tasks");

    for ( int i = 0; i < size; i++ )
    {
        set.setArrayIndex( i );
        Task task ( database );
        task.setCronjob ( set.value("cronjob").toString() );
        task.setDevAddress ( set.value("devAddr").toString() );
        task.setDevType ( set.value("devType").toString() );
        task.setLogin ( set.value("login").toString() );
        task.setPassword ( set.value("password").toString() );
        task.setTask ( set.value("task").toString() );
        task.setPriority( set.value("priority").toInt() );

        QString temp = set.value("arguments").toString();
        QStringList list;
        if(temp != "")
        {
            list = temp.split(",");
            task.setArgums(list);
        }
        else
        {
            task.setArgums(list);
        }
        task.writeTask();
        addTask( task );
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
void TaskManager::clearTaskTable()
{
    if ( database.open() )
    {
        QSqlQuery * query = new QSqlQuery ( database );
        QString str = "DELETE   FROM tasks";
        if ( query->exec(str) )
        {
            qDebug() << "TaskManager.clearTaskTable()]:  table cleared";
        }
        else
        {
            qDebug() << "[TaskManager.clearTaskTable()]:  Query failed! " << query->lastError();
        }

        query->clear();
        str.clear();
        str = "DELETE FROM sqlite_sequence WHERE name = 'tasks'";
        if ( !query->exec(str) )
        {
            qDebug() << "[TaskManager.clearTaskTable()]:  Query failed! " << query->lastError();
        }
    }
    else
    {
        qDebug() << "[TaskManager.clearTaskTable()]:  Error connecting to database!";
    }
}

//--
void TaskManager::checkTaskTable()
{
    if ( database.open() )
    {
        bool empty = true;
        QSqlQuery * query = new QSqlQuery ( database );
        QString str = "SELECT COUNT(task_id) FROM tasks";
        if ( query->exec(str) )
        {
            int res = 0;
            while ( query->next() )
            {
                res = query->value(0).toInt();
            }
            if ( res != 0 )
            {
                empty = false;
            }
        }
        else
        {
            qDebug() << "[TaskManager.checkTaskTable()]:  Query failed! " << query->lastError();
        }

        if ( empty != true )
        {
            query->clear();
            str.clear();
            str = "SELECT * FROM tasks";
            if ( query->exec(str) )
            {
                while ( query->next() )
                {
                    Task task ( database );
                    task.setID( query->value(0).toInt() );
                    task.setCronjob( query->value(1).toString() );
                    task.setDevAddress( query->value(2).toString() );
                    task.setDevType( query->value(3).toString() );
                    task.setLogin( query->value(4).toString() );
                    task.setPassword( query->value(5).toString() );
                    task.setTask( query->value(6).toString() );
                    task.setArgums( query->value(7).toStringList() );
                    task.setPriority( query->value(8).toInt() );
                    task.setStatus( query->value(9).toInt() );
                    addTask(task);
                }
            }
        }
        else
        {
            qDebug() << "[TaskManager.checkTaskTable()]:  Query failed! " << query->lastError();
        }
    }
    else
    {
        qDebug() << "[TaskManager.checkTaskTable()]:  Error connecting to database!";    
    }
}

//--
void TaskManager::slt_OnDone(Task task)
{
    removeTask( task );
}

//--
taskVector_t TaskManager::toTaskVector()
{
    taskVector_t vector;
    for ( int i = 0; i < taskList.size(); i++ )
    {
        QPair <QString, int> pair;
        pair.first = taskList[i].getCronjob();
        pair.second = i;
        vector.append(pair);
    }
    return vector;
}

/*======================================================
метод-слот, запускающий  задачу на выполнение

параметры метода:
    int index - порядковый номер задачи в списке
======================================================*/
void TaskManager::slt_Run (int index )
{
    qDebug() << "[" << QDateTime::currentDateTime().toString("dd/MM/yy hh:mm:ss")
             << "]: Call " << taskList[index].getTask() << " for " << taskList[index].getDevType() << endl;
}

/*======================================================
деструктор класса
======================================================*/
TaskManager::~TaskManager ( )
{

}

