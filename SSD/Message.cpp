#include "Message.h"

//=======================Message================
Message::Message(Model *parent) : Model(parent)
{

}

Message::~Message()
{

}

//=======================Income================
Income::Income(Message *parent) : Message(parent)
{

}

Income::~Income()
{

}

//=======================Outcome================
Outcome::Outcome(Message *parent) : Message(parent)
{

}

Outcome::~Outcome()
{

}
