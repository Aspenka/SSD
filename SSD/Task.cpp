#include "task.h"
#include <QVariant>

Task::Task(QObject *parent) : QObject(parent)
{
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
    status = obj.status;
    uid = obj.uid;
    deviceUid = obj.deviceUid;
    parameters = obj.parameters;
    model = obj.model;
    query = obj.query;
}

bool Task::operator == ( const Task &right )
{
    if ((cronjob == right.cronjob) &&
         (status == right.status) &&
         (parameters == right.parameters) &&
         (model == right.model) &&
         (uid == right.uid) &&
         (deviceUid == right.deviceUid) &&
         (query == right.query))
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

void Task::setUid(int i)
{
    uid = i;
}

void Task::setStatus(int st)
{
    status = st;
}

void Task::setParameters(QString params)
{
    parameters = params;
}

void Task::setCronjob(QString cron)
{
    cronjob = cron;
}

void Task::setDeviceUid(QString uid)
{
    deviceUid = uid;
}

QString Task::getDeviceUid()
{
    return deviceUid;
}

int Task::getUid()
{
    return uid;
}

//--
QString Task::getCronjob()
{
    return cronjob;
}

QString Task::getParameters()
{
    return parameters;
}

void Task::save()
{
    model->setIsNew(true);
    model->setField("status", status);
    model->setField("cronjob", cronjob);
    model->setField("parameters", parameters);
    if(model->save())
    {
        qDebug() << "[TASK]:\tRecord is changed";
    }
}

void Task::remove()
{
    if(model->remove())
    {
        qDebug() << "[TASK]:\tRecord deleted";
    }
}

void Task::edit(int taskStatus)
{
    QStringList list = model->getFields();
    list.removeOne("uid");
    if((taskStatus != 0))
    {
        setStatus(taskStatus);
        model->setField("status", status);
        if(status == DONE)
        {
            emit sig_done(*this);
        }
    }
    if(model->save(list))
    {
        qDebug() << "[TASK]:\tRecord is changed";
    }
}

//--
void Task::print()
{
    qDebug() << "uid:\t" << uid;
    qDebug() << "cronjob:\t" << cronjob;
    qDebug() << "status:\t" << status;
    qDebug() << "device_uid:\t" << deviceUid;
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

    Query *tarQuery = new Query(TargetModel::getTableSchema());
    TargetModel tar = new TargetModel(tarQuery->getEmpty());
    model->setRelation(&tar, "task_to_target");
    query->updateSchema(model);
    QStringList selLlist = tar.getSelectedFields(QStringList({"device_uid"}));
    query->setSelectedFields(selLlist);
    query->withInner("task_to_target");
    model = static_cast <TaskModel *> (query->getOne());
    QList <TargetModel *> deviceList = TargetModel::toTargetModel(model->getRelationData("task_to_target"));
    query->clear();

    for(int i = 0; i < list.size(); i++)
    {
        Task t;
        t.setUid(list[i]->getRecord("uid").toInt());
        t.setCronjob(list[i]->getRecord("cronjob").toString());
        t.setStatus(list[i]->getRecord("status").toInt());

        t.query = query;
        t.model = list[i];
        t.setDeviceUid(deviceList[i]->getRecord("device_uid").toString());
        taskList.append(t);
    }
    return taskList;
}

Task::~Task()
{

}

