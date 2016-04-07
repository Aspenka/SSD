#ifndef Scheduler_H
#define Scheduler_H

#include <QObject>
#include <QVector>
#include <QPair>
#include <QDateTime>
#include <QStringList>
#include <QDebug>
#include "Timer.h"
#include "CronParser.h"

#define taskPair_t        QPair <QString, int>    //один экземпляр задания, имеющего формат {"cron-выражение", "имя задания"}
#define taskVector_t      QVector <QPair <QString, int>>           //вектор заданий формата TaskPair

//класс предназначен для вызова определенных процедур в указанное время, представленное в виде cron-выражения
class Scheduler : public QObject
{
    Q_OBJECT
private:
    taskPair_t  task;              //одно задание
    taskVector_t taskVect;        //перечень заданий
    Timer *timer;               //таймер

public:
    explicit Scheduler(QObject *parent = 0);  //пустой конструктор
    Scheduler(taskPair_t  oneTask, QObject *parent = 0); //конструктор, принимающий один экземпляр задания
    Scheduler(taskVector_t taskVector, QObject *parent = 0); //конструктор, принимающий перечень заданий
    ~Scheduler();        //деструктор

    void append(taskPair_t  oneTask);          //добавить одно задание
    void append(taskVector_t taskVector);     //добавить перечень заданий

    void remove(taskPair_t  oneTask);  //метод удаляет из вектора заданий задание, равное oneTask
    void remove(int index); //метод удаляет из вектора заданий значение с индексом index

    void clear();   //очистить очередь заданий
signals:
    void callTask(int taskIndex);     //сигнал о начале выполнения нового задания
public slots:
    void start();   //метод-слот запускает планировщик
    void slotReaction(taskPair_t  task);   //метод-слот, реагируюшщий на срабатывание таймера
};

#endif // Scheduler_H
