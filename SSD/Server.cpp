#include <cstdlib>
#include "Server.h"

Server::Server ( QObject *parent) : QObject(parent )
{

}

void Server::startServer ( )
{
    taskMan = new TaskManager ( );
    taskMan->run();
}

Server::~Server ( )
{

}

