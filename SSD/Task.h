#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>
#include "TaskModel.h"
#include "Query.h"

enum status_e
{
    NEW_TASK = 1,
    PROCESSING = 2,
    DONE = 3
};

class Task : public QObject
{
    Q_OBJECT
public:  
    explicit Task(QObject *parent = 0);
    Task ( Task const & obj, QObject *parent = 0 );
    ~Task();

    void            setStatus ( int st );
    void            setSingle ( bool sgl );
    void            setPriority ( int pr );
    void            setCronjob ( QString cron );
    void            setDevAddress ( QString devAddr );
    void            setDevType ( QString devType );
    void            setLogin ( QString lgn );
    void            setPassword ( QString pass );
    void            setTask ( QString tsk );
    void            setArgums (QStringList args );
    void            setID (int id);

    int             getPriority();
    bool            getSingle();
    QList <Task>    getTaskList();
    QString         getCronjob();   //--
    QString         getTask();      //--
    QString         getDevType();   //--

    bool            isEmpty();


    void            save();
    void            remove();
    void            edit(int taskStatus = 0, int taskPriority = 0);

    void            print();

    Task &          operator = (Task const & obj);
    bool            operator == (const Task & right);
    bool            operator != (const Task & right);
private:
    QString         cronjob,
                    deviceAddress,
                    deviceType,
                    login,
                    password,
                    task;
    QStringList     taskArguments;
    bool            single;
    int             priority,
                    status,
                    taskID;
    TaskModel       *model;
    Query           *query;

    QString     toString (QStringList list);    //-
    void        copy(const Task &obj);
    Task        parseArgs(Task & task, QList <QString> argList); //заглушка

signals:
    void        sig_done(Task);

public slots:
};

#endif // TASK_H
