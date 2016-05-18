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
};


/*======================================================
 Класс Income
======================================================*/
class Income : public Message
{
public:
    explicit Income(Message *parent = 0);
    ~Income();
};

/*======================================================
 Класс Outcome
======================================================*/
class Outcome : public Message
{
public:
    explicit Outcome(Message *parent = 0);
    ~Outcome();
};
#endif // MESSAGE_H
