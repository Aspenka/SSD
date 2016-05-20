#include <QCoreApplication>
#include "DCServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DCServer *server = new DCServer();
    server->startServer();
    return a.exec();
}
