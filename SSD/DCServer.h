#ifndef DCSERVER_H
#define DCSERVER_H

#include <QObject>
#include <QJsonDocument>
#include "TaskManager.h"
#include "var.h"


//класс, управляющий ПО ССД

class DCServer : public QObject
{
    Q_OBJECT
public:
    explicit DCServer(QObject *parent = 0);
    ~DCServer();
    void            startServer ( );

private:
    //TaskManager         *taskMan;
    void parse(QString tableName, QJsonDocument data);
    template <class T>
    void addToTable(QList <Var> list, T *& model);

signals:

public slots:

};

#endif // DCSERVER_H
