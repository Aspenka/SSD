#ifndef TASKMODEL_H
#define TASKMODEL_H

#include "model.h"
#include "tableschema.h"

class TaskModel : public Model
{
public:
            TaskModel(Model *parent = 0);
            ~TaskModel();

    static  TableSchema *getTableSchema();
    static  QList <TaskModel *> toTaskModel(QList <Model *> modelList);

private:
};

#endif // TASKMODEL_H
