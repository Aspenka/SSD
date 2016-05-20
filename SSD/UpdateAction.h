#ifndef UPDATEACTION_H
#define UPDATEACTION_H

#include <QJsonDocument>
#include "Action.h"

class UpdateAction : public Action
{
public:
    UpdateAction();
    ~UpdateAction();

    virtual void execute(QJsonDocument data);
};

#endif // UPDATEACTION_H
