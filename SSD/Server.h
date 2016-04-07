#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include "TaskManager.h"

//класс, управляющий ПО ССД

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();
    void            startServer ( );

private:
    TaskManager     *taskMan;

signals:

public slots:

};

#endif // SERVER_H
