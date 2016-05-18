#include <cstdlib>
#include "DCServer.h"
#include "TaskManager.h"
#include "Request.h"

DCServer::DCServer ( QObject *parent) : QObject(parent )
{

}

void DCServer::startServer ( )
{
    //taskMan = new TaskManager ( );
    //taskMan->run();
    //taskMan->print();
    Request *r = new Request("http://pvo.ascae.ru/api/sync/get?represent=api/device/get-dcs&access-token=api_token");
}

DCServer::~DCServer ( )
{

}

