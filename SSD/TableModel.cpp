#include "TableModel.h"
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QVariant>

//===================TaskModel=====================
TaskModel::TaskModel(Model *parent) : Model(parent)
{
    if(parent != 0)
    {
        copy(*parent);
    }
    isNew = true;
    relation.link = {"task.uid", "task_uid"};
    relation.type = ONE_TO_ONE;
    schema = getTableSchema();
}

TaskModel * TaskModel::getModel()
{
    return this;
}

TableSchema * TaskModel::getTableSchema()
{
    TableSchema *schem = new TableSchema();
    schem->setTableName("task");
    QStringList list {
                        "uid",
                        "cronjob",
                        "parameters",
                        "status"
                      };
    schem->setFields(list);
    schem->setPrimaryKey(QStringList({"uid"}));
    schem->setForeignKey(QStringList({"uid"}));
    return schem;
}

QList <TaskModel *> TaskModel::toTaskModel(QList<Model *> modelList)
{
    QList <TaskModel *> list;
    for(int i = 0; i < modelList.size(); ++i)
    {
        list.append(static_cast <TaskModel *> (modelList[i]));
    }
    return list;
}

TaskModel::~TaskModel()
{

}

//===================TargetModel=====================
TargetModel::TargetModel(Model *parent) : Model(parent)
{
    if(parent != 0)
    {
        copy(*parent);
    }
    isNew = true;
    //relation.link = {"target.task_uid", "task.uid"};
    //relation.type = ONE_TO_ONE;
    schema = getTableSchema();
}

TargetModel * TargetModel::getModel()
{
    return this;
}

TableSchema * TargetModel::getTableSchema()
{
    TableSchema *schem = new TableSchema();
    schem->setTableName("target");
    QStringList list {
                        "uid",
                        "task_uid",
                        "device_uid"
                      };
    schem->setFields(list);
    schem->setPrimaryKey(QStringList({"uid"}));
    schem->setForeignKey(QStringList({"task_uid", "device_uid"}));
    return schem;
}

QList <TargetModel *> TargetModel::toTargetModel(QList<Model *> modelList)
{
    QList <TargetModel *> list;
    for(int i = 0; i < modelList.size(); i++)
    {
        list.append(static_cast <TargetModel *> (modelList[i]));
    }
    return list;
}

TargetModel::~TargetModel()
{

}

//=================DeviceModel=======================
DeviceModel::DeviceModel(Model *parent) : Model(parent)
{
    if(parent != 0)
    {
        copy(*parent);
    }
    isNew = true;
    relation.link = {"device_id", "device"};
    relation.type = ONE_TO_ONE;
    schema = getTableSchema();
}

DeviceModel * DeviceModel::getModel()
{
    return this;
}

TableSchema * DeviceModel::getTableSchema()
{
    TableSchema *schem = new TableSchema();
    schem->setTableName("device");
    QStringList list {
                        "device_uid"
                      };
    schem->setFields(list);
    schem->setPrimaryKey(QStringList({"device_uid"}));
    schem->setForeignKey(QStringList({"device_uid"}));
    return schem;
}

QList <DeviceModel *> DeviceModel::toDeviceModel(QList<Model *> modelList)
{
    QList <DeviceModel *> list;
    for(int i = 0; i < modelList.size(); i++)
    {
        list.append(static_cast <DeviceModel *> (modelList[i]));
    }
    return list;
}

DeviceModel::~DeviceModel()
{

}

//=================NetAddressTypeModel=======================
NetAddressTypeModel::NetAddressTypeModel(Model *parent) : Model(parent)
{
    if(parent != 0)
    {
        copy(*parent);
    }
    isNew = true;
    //relation.link = {"device_id", "device"};
    //relation.type = ONE_TO_ONE;
    schema = getTableSchema();
}

NetAddressTypeModel * NetAddressTypeModel::getModel()
{
    return this;
}

TableSchema * NetAddressTypeModel::getTableSchema()
{
    TableSchema *schem = new TableSchema();
    schem->setTableName("net_address_type");
    QStringList list {
                        "net_address_type_uid",
                        "name_ru"
                      };
    schem->setFields(list);
    schem->setPrimaryKey(QStringList({"net_address_type_uid"}));
    schem->setForeignKey(QStringList({"net_address_type_uid"}));
    return schem;
}

QList <NetAddressTypeModel * > NetAddressTypeModel::toNetArrdessTypeModel(QList<Model *> modelList)
{
    QList <NetAddressTypeModel *> list;
    for(int i = 0; i < modelList.size(); i++)
    {
        list.append(static_cast <NetAddressTypeModel *> (modelList[i]));
    }
    return list;
}

NetAddressTypeModel::~NetAddressTypeModel()
{

}

//=================NetAddressModel=======================
NetAddressModel::NetAddressModel(Model *parent) : Model(parent)
{
    if(parent != 0)
    {
        copy(*parent);
    }
    isNew = true;
    //relation.link = {"device_id", "device"};
    //relation.type = ONE_TO_ONE;
    schema = getTableSchema();
}

NetAddressModel * NetAddressModel::getModel()
{
    return this;
}

TableSchema * NetAddressModel::getTableSchema()
{
    TableSchema *schem = new TableSchema();
    schem->setTableName("net_address");
    QStringList list {
                        "net_address_uid",
                        "value",
                        "priority",
                        "net_address_type_uid",
                        "device_uid"
                      };
    schem->setFields(list);
    schem->setPrimaryKey(QStringList({"net_address_uid"}));
    schem->setForeignKey(QStringList({"net_address_type_uid, device_uid"}));
    return schem;
}

QList <NetAddressModel * > NetAddressModel::toNetArrdessModel(QList<Model *> modelList)
{
    QList <NetAddressModel *> list;
    for(int i = 0; i < modelList.size(); i++)
    {
        list.append(static_cast <NetAddressModel *> (modelList[i]));
    }
    return list;
}

NetAddressModel::~NetAddressModel()
{

}
