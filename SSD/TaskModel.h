#ifndef TASKMODEL_H
#define TASKMODEL_H

#include "model.h"
#include "tableschema.h"

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

class TargetModel : public Model
{
public:
    TargetModel (Model *parent = 0);
    ~TargetModel();

    static TableSchema * getTableSchema();
    static QList <TargetModel *> toTargetModel (QList <Model *> modelList);
    TargetModel * getModel();
};

class DeviceModel : public Model
{
public:
    DeviceModel (Model *parent = 0);
    ~DeviceModel();

    static TableSchema * getTableSchema();
    static QList <DeviceModel *> toDeviceModel (QList <Model *> modelList);
    DeviceModel * getModel();
};

class NaType

/*class IncomeModel : public Model
{
    IncomeModel (Model *parent = 0);
    ~IncomeModel();

    static TableSchema * getTableSchema();
    static QList <IncomeModel *> toIncomeModel (QList <Model *> modelList);
    IncomeModel * getModel();
};*/
#endif // TASKMODEL_H
