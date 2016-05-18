#include <QCoreApplication>
#include "DCServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DCServer serv;
    serv.startServer();
    return a.exec();
}
