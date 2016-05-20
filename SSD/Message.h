#ifndef MESSAGE_H
#define MESSAGE_H

#include "Model.h"
/*======================================================
 Класс Message
======================================================*/
class Message : public Model
{
public:
    explicit Message(Model *parent = 0);
    ~Message();
protected:
    bool    received,
            processed;
    QString errorText;

    void    setReceived(bool status);
    void    setProcessed(bool status);
    void    setErrorText(QString text);
};


/*======================================================
 Класс Income
======================================================*/
class Income : public Message
{
public:
    explicit Income(Message *parent = 0);
    ~Income();

    static TableSchema *getTableSchema();
    static QList <Income *> toIncome(QList <Message *> modelList);
    Income * getModel();
    void    execute();
};

/*======================================================
 Класс Outcome
======================================================*/
class Outcome : public Message
{
public:
    explicit Outcome(Message *parent = 0);
    ~Outcome();

    static TableSchema *getTableSchema();
    static QList <Outcome *> toIncome(QList <Message *> modelList);
    Outcome * getModel();
    void    execute();
    void    send();
};
#endif // MESSAGE_H
