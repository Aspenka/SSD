#include <QDebug>
#include <QSqlQuery>
#include <stdlib.h>
#include "Task.h"

/*======================================================
конструктор класса
======================================================*/
Task::Task (const QSqlDatabase &db, QObject *parent ) : QObject ( parent )
{
    id = 0;
    database = db;
    setSingle ( false );
    setStatus ( NEW_TASK );
}

Task::Task ( QObject *parent ) : QObject ( parent )
{
    id = 0;
    setSingle ( false );
    setStatus ( NEW_TASK );\
}

/*======================================================
конструктор копирования
======================================================*/
Task::Task ( const Task &obj, QObject *parent) : QObject ( parent )
{
    cronjob = obj.cronjob;
    deviceAddress = obj.deviceAddress;
    deviceType = obj.deviceType;
    login = obj.login;
    password = obj.password;
    task = obj.task;
    taskArguments = obj.taskArguments;
    priority = obj.priority;
    status = obj.status;
    id = obj.id;
    database = obj.database;
    single = obj.single;
}

/*======================================================
метод устанавливает аргументы задачи

параметры метода:
    QStringList args - список аргументов задачи
======================================================*/
void Task::setArgums ( QStringList args )
{
    taskArguments = args;
}

/*======================================================
метод устанавливает cron-выражение задачи

параметры метода:
    QString cron - строка cron-выражения
======================================================*/
void Task::setCronjob ( QString cron )
{
    cronjob = cron;
}

/*======================================================
метод устанавливает адрес подключаемого устройства

параметры метода:
    QString devAddr - адрес устройства
======================================================*/
void Task::setDevAddress ( QString devAddr )
{
    deviceAddress = devAddr;
}

/*======================================================
метод устанавливает тип подключаемого устройства

параметры метода:
    QString devTypr - тип устройства
======================================================*/
void Task::setDevType ( QString devType )
{
    deviceType = devType;
}

/*======================================================
метод устанавливает логин подключения к УСПД

параметры метода:
    QString lgn - логин
======================================================*/
void Task::setLogin ( QString lgn )
{
    login = lgn;
}

/*======================================================
метод устанавливает пароль подключения к УСПД

параметры метода:
    QString pass - пароль
======================================================*/
void Task::setPassword ( QString pass )
{
    password = pass;
}

/*======================================================
метод устанавливает приоритет задания

параметры метода:
    int pr - приоритет задачи
======================================================*/
void Task::setPriority ( int pr )
{
    priority = pr;
}

/*======================================================
метод устанавливает статус задания

параметры метода:
    int st - статус задачи
======================================================*/
void Task::setStatus ( int st )
{
    status = st;
}

//--
void Task::setSingle(bool sgl)
{
    single = sgl;
}

/*======================================================
метод устанавливает задачу

параметры метода:
    QString tsk - строка, описывающая задачу
======================================================*/
void Task::setTask ( QString tsk )
{
    task = tsk;
}

//--
void Task::setID(int baseID)
{
    id = baseID;
}

/*======================================================
метод возвращает приоритет задачи
======================================================*/
int Task::getPriority ()
{
    return priority;
}

/*======================================================
метод возвращает статус задачи
======================================================*/
int Task::getStatus ()
{
    return status;
}

//--
bool Task::getSingle()
{
    return single;
}

//--
QString Task::getCronjob()
{
    return cronjob;
}

//--
QString Task::getTask()
{
    return task;
}

//--
QString Task::getDevType()
{
    return deviceType;
}

/*======================================================
метод удаляет задачу из БД
======================================================*/
void Task::removeTask ()
{
    try
    {
        if ( database.open() )
        {
            QSqlQuery *query = new QSqlQuery ( database );
            QString str = "DELETE FROM tasks WHERE task_id = :id";
            query->prepare(str);
            query->bindValue(":id", id);
            try
            {
                if ( query->exec() )
                {
                    qDebug() <<  "[Task.removeTask()]:  Record " << id << " deleted";
                }
                else
                {
                    throw query;
                }
            }
            catch (QSqlQuery *)
            {
                qDebug() << "[Task.removeTask()]:    Query failed! " << query->lastError();
            }
        }
        else
        {
            throw database;
        }
    }
    catch (QSqlDatabase)
    {
        qDebug() << "[Task.removeTask()]:    Error connecting to database!";
    }
}

/*======================================================
метод записывает задачу в БД
======================================================*/
void Task::writeTask ()
{
    try
    {
        if( database.open() )
        {
            QSqlQuery *query = new QSqlQuery ( database );
            QString str = "INSERT INTO tasks (cronjob, dev_address, dev_type, login, password,"
                          " task, task_arg, priority, status) VALUES (:cron, :devAddr, :devType, :login,"
                          " :pass, :task, :taskArg, :pr, :st)";

            query->prepare(str);
            query = bindValues( query );
            try
            {
                if(query->exec())
                {
                    setIdFromBase();
                    qDebug() << "[Task.writeTask()]:    Data recording is successful";
                }
                else
                {
                    throw query;
                }
            }
            catch (QSqlQuery *)
            {
                qDebug() << "[Task.writeTask()]:    Query failed! " << query->lastError();
            }
        }
        else
        {
            throw database;
        }
    }
    catch (QSqlDatabase)
    {
        qDebug() << "[Task.writeTask()]:    Error connecting to database!";
    }
}

//--
QString Task::toString ( QStringList list )
{
    QString result = "";
    if(!list.empty())
    {
        for ( int i = 0; i < list.size(); i++ )
        {
            result += list.at(i);
            result += ",";
        }
    }
    return result;
}

//--
QSqlQuery * Task::bindValues(QSqlQuery * & query)
{
    query->bindValue ( ":cron", cronjob );
    query->bindValue ( ":devAddr", deviceAddress );
    query->bindValue ( ":devType", deviceType );
    query->bindValue ( ":login", login );
    query->bindValue ( ":pass", password );
    query->bindValue ( ":task", task );
    query->bindValue ( ":pr", priority );
    query->bindValue ( ":st", status );
    QString args = toString ( taskArguments );
    query->bindValue ( ":taskArg", args );
    return query;
}

//-
void Task::setIdFromBase()
{
    try
    {
        if( database.open() )
        {
            QSqlQuery *query = new QSqlQuery ( database );
            QString str = "SELECT MAX(task_id) FROM tasks";
            try
            {
                if ( query->exec(str) )
                {
                    while ( query->next() )
                    {
                        setID ( query->value( 0 ).toInt() );
                    }
                }
                else
                {
                    throw query;
                }
            }
            catch (QSqlQuery *)
            {
                qDebug() << "[setIdFromBase()]:    Query failed! " << query->lastError();
            }
        }
        else
        {
            throw database;
        }
    }
    catch (QSqlDatabase)
    {
        qDebug() << "[setIdFromBase()]:    Error connecting to database!";
    }
}

/*======================================================
метод редактирует задачу в БД
примечание: редактировать позволено только статус и
            приритет задачи
======================================================*/
void Task::editTask (int st, int pr)
{
    try
    {
        if( database.open() )
        {
            QSqlQuery *query = new QSqlQuery ( database );
            QString str = "";

            if ( (pr == 0) && (st == 0) )
            {
                return;
            }
            if ( (pr== 0) && (st != 0) )
            {
                if ( status == DONE )
                {
                    emit sig_done( * this );
                    return;
                }
                str = "UPDATE tasks SET status = :st WHERE task_id = :id";
            }
            if ( (pr != 0) && (st == 0) )
            {
                str = "UPDATE tasks SET priority = :pr WHERE task_id = :id";
            }
            if ( (pr != 0) && (st != 0) )
            {
                if ( status == DONE )
                {
                    qDebug() << "Emit DONE!";
                    emit sig_done( * this );
                    return;
                }
                str = "UPDATE tasks SET status = :st, priority = :pr WHERE task_id = :id";
            }

            query->prepare( str );
            query->bindValue(":st", status);
            query->bindValue(":pr", priority);
            query->bindValue(":id", id);

            try
            {
                if ( query->exec() )
                {
                    qDebug() << "[Task.editTask()]: Record " << id << " changed";
                }
                else
                {
                    throw query;
                }
            }
            catch (QSqlQuery *)
            {
                qDebug() << "[Task.editTask()]: Query failed! " << query->lastError();
            }
        }
        else
        {
            throw database;
        }
    }
    catch (QSqlDatabase)
    {
       qDebug() << "[Task.editTask()]: Error connecting to database!";
    }
}

//заглушка
void Task::print ()
{
    qDebug() << "ID: " << id;
    qDebug() << "cronjob: " << cronjob;
    qDebug() << "device address: " << deviceAddress;
    qDebug() << "device type: " << deviceType;
    qDebug() << "login: " << login;
    qDebug() << "password: " << password;
    qDebug() << "task: " << task;
    if(!taskArguments.empty())
    {
        qDebug() << "Arguments: ";
        for( int i = 0; i < taskArguments.size(); i++ )
            qDebug() << "arg " << i << ": " << taskArguments.at(i);
    }
    else
    {
        qDebug() << "Arguments: none";
    }
    qDebug() << "status: " << status;
    qDebug() << "priority: " << priority;
}

/*======================================================
метод предназнчаен для копирования атрибутов одного
объекта класса Task в другой объект класса Task
======================================================*/
void Task::swap ( Task & obj )
{
    std::swap ( cronjob, obj.cronjob );
    std::swap ( deviceAddress, obj.deviceAddress );
    std::swap ( deviceType, obj.deviceType );
    std::swap ( login, obj.login );
    std::swap ( password, obj.password );
    std::swap ( task, obj.task );
    std::swap ( taskArguments, obj.taskArguments );
    std::swap ( priority, obj.priority );
    std::swap ( status, obj.status );
    std::swap ( id, obj.id );
    std::swap ( database, obj.database );
    std::swap ( single, obj.single );
}

/*======================================================
метод перегружает оператор = (оператор присваивания),
позволяя присваивать объекту Task значение другого
объекта Task
======================================================*/
Task & Task::operator = ( Task const & obj )
{
    if( this != & obj )
    {
        Task ( obj ).swap( * this );
    }
    return *this;
}

/*======================================================
метод перегружает оператор == (оператор сравнения),
позволяя сравнивать между собой объекты типа Task.
Если объекты равны, метод возвращает true, иначе - false
======================================================*/
bool Task::operator == ( const Task &right )
{
    if ( ( cronjob == right.cronjob ) &&
         ( deviceAddress == right.deviceAddress ) &&
         ( deviceType == right.deviceType ) &&
         ( login == right.login ) &&
         ( password == right.password ) &&
         ( task == right.task ) &&
         ( taskArguments == right.taskArguments ) &&
         ( priority == right.priority ) &&
         ( status == right.status ) &&
         ( single == right.single))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*======================================================
метод перегружает оператор != (не равно), позволяя
сравнивать между собой объекты типа Task.
Если объекты Task не равны друг другу, метод возвращает
true, иначе - false
======================================================*/
bool Task::operator != ( const Task & right )
{
    return !( this == & right );
}

/*======================================================
деструктор класса
======================================================*/
Task::~Task()
{

}

