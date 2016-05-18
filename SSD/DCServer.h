#ifndef DCSERVER_H
#define DCSERVER_H

#include <QObject>
#include "TaskManager.h"

//класс, управляющий ПО ССД

class DCServer : public QObject
{
    Q_OBJECT
public:
    explicit DCServer(QObject *parent = 0);
    ~DCServer();
    void            startServer ( );

private:
    TaskManager     *taskMan;

signals:

public slots:

};

#endif // DCSERVER_H
