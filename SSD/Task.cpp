#include "task.h"
#include <QVariant>

Task::Task(QObject *parent) : QObject(parent)
{
    setSingle(false);
    setStatus(NEW_TASK);
    query = new Query(TaskModel::getTableSchema());
    model = new TaskModel(query->getEmpty());
}

Task::Task(const Task &obj, QObject *parent) : QObject(parent)
{
    copy(obj);
}

void Task::copy(const Task &obj)
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
    single = obj.single;
    taskID = obj.taskID;
    model = obj.model;
    query = obj.query;
}

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
         ( single == right.single) &&
         ( taskID == right.taskID) &&
         ( model == right.model) &&
         ( query == right.query))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Task::operator != ( const Task & right )
{
    return !( this == & right );
}

Task & Task::operator = ( Task const & obj )
{
    copy(obj);
    return *this;
}

void Task::setStatus(int st)
{
    status = st;    
}

void Task::setSingle(bool sgl)
{
    single = sgl;
}

void Task::setPriority(int pr)
{
    priority = pr;
}

void Task::setCronjob(QString cron)
{
    cronjob = cron;
}

void Task::setDevAddress(QString devAddr)
{
    deviceAddress = devAddr;
}

void Task::setDevType(QString devType)
{
    deviceType = devType;
}

void Task::setLogin(QString lgn)
{
    login = lgn;    
}

void Task::setPassword(QString pass)
{
    password = pass;
}

void Task::setTask(QString tsk)
{
    task = tsk;
}

void Task::setArgums(QStringList args)
{
    taskArguments = args;
    *this = parseArgs(*this, args);
}

void Task::setID(int id)
{
    taskID = id;
}

int Task::getPriority()
{
    return priority;
}

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

void Task::save()
{
    model->setIsNew(true);
    model->setField("status", status);
    model->setField("priority", priority);
    model->setField("cronjob", cronjob);
    model->setField("dev_address", deviceAddress);
    model->setField("dev_type", deviceType);
    model->setField("login", login);
    model->setField("password", password);
    model->setField("task", task);
    if(!taskArguments.empty())
    {
        model->setField("task_arg", toString(taskArguments));
    }
    else
    {
        model->setField("task_arg", "");
    }
    if(model->save())
    {
        qDebug() << "[Task::save]:\tData recorded";
        setID(query->getlastID().toInt());
        model->setField("task_id", taskID);
    }
}

void Task::remove()
{
    if(model->remove())
    {
        qDebug() << "[Task::remove]:\tRecord deleted";
    }
}

void Task::edit(int taskStatus, int taskPriority)
{
    QStringList list = model->getFields();
    list.removeOne("task_id");
    if((taskStatus == 0) && (taskPriority !=0))
    {
        setPriority(taskPriority);
        model->setField("priority", priority);
    }
    if((taskStatus != 0) && (taskPriority == 0))
    {
        setStatus(taskStatus);
        model->setField("status", status);
    }
    if((taskStatus != 0) && (taskPriority != 0))
    {
        setPriority(taskPriority);
        setStatus(taskStatus);
        model->setField("priority", priority);
        model->setField("status", status);
    }
    if(model->save(list))
    {
        qDebug() << "[Task::edit]:\tRecord is changed";
    }
    if(status == DONE)
    {
        emit sig_done(*this);
    }
}

//--
void Task::print()
{
    qDebug() << "ID:\t" << taskID;
    qDebug() << "cronjob:\t" << cronjob;
    qDebug() << "device address:\t" << deviceAddress;
    qDebug() << "device type:\t" << deviceType;
    qDebug() << "login:\t" << login;
    qDebug() << "password:\t" << password;
    qDebug() << "task:\t" << task;
    if(!taskArguments.empty())
    {
        qDebug() << "Arguments:\t";
        for( int i = 0; i < taskArguments.size(); i++ )
        {
            qDebug() << "arg\t" << i << ": " << taskArguments.at(i);
        }
    }
    else
    {
        qDebug() << "Arguments: none";
    }
    qDebug() << "status:\t" << status;
    qDebug() << "priority:\t" << priority;
    qDebug() << "single:\t" << single;
}

bool Task::isEmpty()
{
    if(query->count() != 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

QList<Task> Task::getTaskList()
{
    QList <Task> taskList;
    query->clear();
    QList <TaskModel *> list = TaskModel::toTaskModel(query->getAll());
    for(int i = 0; i < list.size(); i++)
    {
        Task t;
        t.setID(list[i]->getRecord("task_id").toInt());
        t.setCronjob(list[i]->getRecord("cronjob").toString());
        t.setDevType(list[i]->getRecord("dev_type").toString());
        t.setDevAddress(list[i]->getRecord("dev_address").toString());
        t.setLogin(list[i]->getRecord("login").toString());
        t.setPassword(list[i]->getRecord("password").toString());
        t.setTask(list[i]->getRecord("task").toString());
        t.setArgums(list[i]->getRecord("task_arg").toStringList());
        t.setPriority(list[i]->getRecord("priority").toInt());
        t.setStatus(list[i]->getRecord("status").toInt());

        t.query = query;
        t.model = list[i];
        taskList.append(t);
    }
    return taskList;
}

//--
QString Task::toString(QStringList list)
{
    QString result = "";
    int i=0;
    if(!list.empty())
    {
        result += list[i];
        i++;
        while( i < list.size() )
        {
            result += ", ";
            result += list[i];
            i++;
        }
    }
    return result;
}

//заглушка
Task Task::parseArgs(Task &task, QList<QString> argList)
{
    for ( int i = 0; i < argList.size(); i++ )
    {
        if ( argList[i] == "single" )
        {
            task.setSingle( true );
        }
    }
    return task;
}

Task::~Task()
{

}

