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

/*=====================================================================
 Класс предназначен для вызова определенных процедур в указанное время,
 представленное в виде cron-выражения

 Атрибуты класса:
 1) timer - таймер, отслеживающий время запуска задачи;
 2) oneTask - крнкретная задача;
 3) taskList - список задач;

 Методы класса:
 1) Scheduler() - конструктор по умолчанию;
 2) Scheduler(Task  & oneTsk) - конструктор, принимающий один экземпляр
                                задания;
 3) Scheduler(QList <Task> & taskLst) - конструктор, принимающий
                                        перечень заданий;
 4) ~Scheduler() - деструктор;
 5) void append(Task & oneTsk) - метод добавляет в список обрабатываемых
                                 задач одну задачу;
 6) void append(QList <Task> & taskLit) - метод добавляет с писок
                                          перечень задач;
 7) void remove(Task & oneTsk) - метод удаляет из вектора заданий
                                 задание, равное oneTask;
 8) void remove(int index) - метод удаляет из вектора заданий значение
                             с индексом index ;
 9) void clear() - метод очищает очередь заданий.

 Сигналы класса:
 1) void sig_callTask(int taskIndex) - сигнал о начале выполнения
                                       нового задания;
 2) void sigDone(int taskIndex) - сигнал о начале выполнения задачи,
                                  пересылающийся в объект класса
                                  TaskManager

 Слоты класса:
 1) void start() - метод-слот запускает планировщик;
 2) void restart() - метод перезапускает планировщик задач;
 3) void slt_Reaction(int index) - метод реагирует на срабатывание
                                   таймера;
 4) void stop() - метод останавливает планировщик.
 =====================================================================*/
class Scheduler : public QObject
{
    Q_OBJECT
public:
    explicit Scheduler(QObject *parent = 0);
    Scheduler(Task  & oneTsk, QObject *parent = 0);
    Scheduler(QList <Task> & taskLst, QObject *parent = 0);
    ~Scheduler();

    void append(Task & oneTsk);
    void append(QList <Task> & taskLit);

    void remove(Task & oneTsk);
    void remove(int index);

    void clear();

private:
    QVector <Timer *> timer;
    Task oneTask;
    QList <Task> taskList;

signals:
    void sig_callTask(int taskIndex);
    void sigDone(int taskIndex);
public slots:
    void start();
    void restart();
    void stop();
    void slt_Reaction(int index);
};

#endif // Scheduler_H
