#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include "model.h"
#include "tableschema.h"

/*======================================================
 Класс TaskModel
======================================================*/
class TaskModel : public Model
{
public:
    TaskModel(Model *parent = 0);
    ~TaskModel();

    static TableSchema *getTableSchema();
    static QList <TaskModel *> toTaskModel(QList <Model *> modelList);
    TaskModel * getModel();

private:
};

/*======================================================
 Класс TargetModel
======================================================*/
class TargetModel : public Model
{
public:
    TargetModel (Model *parent = 0);
    ~TargetModel();

    static TableSchema * getTableSchema();
    static QList <TargetModel *> toTargetModel (QList <Model *> modelList);
    TargetModel * getModel();
};

/*======================================================
 Класс DeviceModel
======================================================*/
class DeviceModel : public Model
{
public:
    DeviceModel (Model *parent = 0);
    ~DeviceModel();

    static TableSchema * getTableSchema();
    static QList <DeviceModel *> toDeviceModel (QList <Model *> modelList);
    DeviceModel * getModel();
};

/*======================================================
 Класс NetAddressTypeModel
======================================================*/
class NetAddressTypeModel : public Model
{
public:
    NetAddressTypeModel (Model *parent = 0);
    ~NetAddressTypeModel();

    static TableSchema * getTableSchema();
    static QList <NetAddressTypeModel *> toNetArrdessTypeModel (QList <Model *> modelList);
    NetAddressTypeModel * getModel();
};

/*======================================================
 Класс NetAddressModel
======================================================*/
class NetAddressModel : public Model
{
public:
    NetAddressModel (Model *parent = 0);
    ~NetAddressModel();

    static TableSchema * getTableSchema();
    static QList <NetAddressModel *> toNetArrdessModel (QList <Model *> modelList);
    NetAddressModel * getModel();
};

#endif // TABLEMODEL_H
