#include "Message.h"

//=======================Message================
Message::Message(Model *parent) : Model(parent)
{
    received = false;
    processed = false;
    errorText = "";
}

void Message::setReceived(bool status)
{
    received = status;
}

void Message::setProcessed(bool status)
{
    processed = status;
}

void Message::setErrorText(QString text)
{
    errorText = text;
}

Message::~Message()
{

}

//=======================Income================
Income::Income(Message *parent) : Message(parent)
{
    if(parent != 0)
    {
        copy(*parent);
    }
    isNew = true;
    //relation.link = {"task.uid", "task_uid"};
    //relation.type = ONE_TO_ONE;
    schema = getTableSchema();
}

Income * Income::getModel()
{
    return this;
}

TableSchema * Income::getTableSchema()
{
    TableSchema *schem = new TableSchema();
    schem->setTableName("income_message");
    QStringList list {
                        "uid",
                        "text",
                        "received_status",
                        "processed_status",
                        "device_uid",
                        "error"
                      };
    schem->setFields(list);
    schem->setPrimaryKey(QStringList({"uid"}));
    schem->setForeignKey(QStringList({"device_uid"}));
    return schem;
}

QList <Income *> Income::toIncome(QList<Message *> modelList)
{
    QList <Income *> list;
    for(int i = 0; i < modelList.size(); ++i)
    {
        list.append(static_cast <Income *> (modelList[i]));
    }
    return list;
}

void Income::execute()
{

}

Income::~Income()
{

}

//=======================Outcome================
Outcome::Outcome(Message *parent) : Message(parent)
{
    if(parent != 0)
    {
        copy(*parent);
    }
    isNew = true;
    //relation.link = {"task.uid", "task_uid"};
    //relation.type = ONE_TO_ONE;
    schema = getTableSchema();
}

Outcome * Outcome::getModel()
{
    return this;
}

TableSchema * Outcome::getTableSchema()
{
    TableSchema *schem = new TableSchema();
    schem->setTableName("outcome_message");
    QStringList list {
                        "uid",
                        "text",
                        "received_status",
                        "processed_status",
                        "device_uid",
                        "error"
                      };
    schem->setFields(list);
    schem->setPrimaryKey(QStringList({"uid"}));
    schem->setForeignKey(QStringList({"device_uid"}));
    return schem;
}

QList <Outcome *> Outcome::toIncome(QList<Message *> modelList)
{
    QList <Outcome *> list;
    for(int i = 0; i < modelList.size(); ++i)
    {
        list.append(static_cast <Outcome *> (modelList[i]));
    }
    return list;
}

void Outcome::send()
{

}

Outcome::~Outcome()
{

}
