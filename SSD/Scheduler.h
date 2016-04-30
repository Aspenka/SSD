#ifndef Scheduler_H
#define Scheduler_H

#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QStringList>
#include <QDebug>
#include "Timer.h"
#include "Task.h"
#include "CronParser.h"

//класс предназначен для вызова определенных процедур в указанное время, представленное в виде cron-выражения
class Scheduler : public QObject
{
    Q_OBJECT
public:
    explicit Scheduler(QObject *parent = 0);  //пустой конструктор
    Scheduler(Task  & oneTsk, QObject *parent = 0); //конструктор, принимающий один экземпляр задания
    Scheduler(QList <Task> & taskLst, QObject *parent = 0); //конструктор, принимающий перечень заданий
    ~Scheduler();        //деструктор

    void append(Task & oneTsk);          //добавить одно задание
    void append(QList <Task> & taskLit);     //добавить перечень заданий

    void remove(Task & oneTsk);  //метод удаляет из вектора заданий задание, равное oneTask
    void remove(int index); //метод удаляет из вектора заданий значение с индексом index

    void clear();   //очистить очередь заданий

private:
    QVector <Timer *> timer;
    Task oneTask;
    QList <Task> taskList;

signals:
    void sig_callTask(int taskIndex);     //сигнал о начале выполнения нового задания
public slots:
    void start();   //метод-слот запускает планировщик
    void restart(); //перезапуск шедулера
    void stop();    //метод останавливает шщедулер
    void slt_Reaction(int index);   //метод-слот, реагируюшщий на срабатывание таймера
};

#endif // Scheduler_H
