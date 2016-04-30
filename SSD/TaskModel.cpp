#include "taskmodel.h"
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QVariant>

TaskModel::TaskModel(Model *parent) : Model(parent)
{
    isNew = true;
    schema = getTableSchema();
    relation.link.insert("dev_type", "uspd_id");
    relation.type = ONE_TO_ONE;
    if(parent != 0)
    {
        copy(*parent);
    }
}

TableSchema * TaskModel::getTableSchema()
{
    TableSchema *schem = new TableSchema();
    schem->setTableName("tasks");
    QStringList list {
                        "task_id",
                        "cronjob",
                        "dev_address",
                        "dev_type",
                        "login",
                        "password",
                        "task",
                        "task_arg",
                        "priority",
                        "status"
                      };
    schem->setFields(list);
    list.clear();
    list.append("task_id");
    schem->setPrimaryKey(list);
    list.clear();
    list.append("dev_type");
    schem->setForeignKey(list);
    return schem;
}

QList <TaskModel *> TaskModel::toTaskModel(QList<Model *> modelList)
{
    QList <TaskModel *> list;
    for(int i = 0; i < modelList.size(); i++)
    {
        list.append(static_cast <TaskModel *> (modelList[i]));
    }
    return list;
}

TaskModel::~TaskModel()
{

}
