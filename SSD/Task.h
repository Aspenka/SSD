#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>
#include "TableModel.h"
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

    void            setStatus(int st);
    void            setUid(int i);    
    void            setCronjob(QString cron);
    void            setParameters(QString params);

    int             getUid();
    QList <Task>    getTaskList();
    QString         getCronjob();
    QString         getParameters();

    bool            isEmpty();

    void            save();
    void            remove();
    void            edit(int taskStatus);

    void            print();

    Task &          operator = (Task const & obj);
    bool            operator == (const Task & right);
    bool            operator != (const Task & right);
private:
    QString         cronjob,
                    parameters;
    int             status,
                    uid;
    TaskModel       *model;
    Query           *query;

    void        copy(const Task &obj);

signals:
    void        sig_done(Task);

public slots:

};

#endif // TASK_H
