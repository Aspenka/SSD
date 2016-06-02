#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>
#include <QList>       
#include "Task.h"
#include "Scheduler.h"

/*===========================================================================
 Класс TaskManager предназначен для управления задачами,
 поступающими из Центрального сервера.
 Основные функции класса:
    -обработка пакета задач, поступающего из ЦС;
    -формирование списка команд для УСПД;
    -управление сформированным списком команд;
    -составление расписания выполнения команд;
    -вызов команд в соответствии с расписанием.

 атрибуты класса:
    taskList - список задач;
    sheduler - объект планировщика задач;
    limit - предельное значение на одновременную
            обработку задач

 методы класса:
void addTask(Task &task) - метод добавляет новую задачу в список;
void removeTask(int index)  - метод удаляет задачу из списка по
                              индексу;
void removeTask(Task task) - метод удаляет задачу из списка по
                             содержимому;
void clear() - метод очищает перечень задач;
void print() - метод отображает содержимое списка задач в консоль;
void run() - метод запускает обработчик задач;
void handleTask() - метод запускает задачу в обработку;
void updateTask (int index, int status = 0) - метод изменяет статус
                                              задачи
void setConfig() - метод устанавливает параметры обработчика задач
                   из конфигурационного файла
слоты класса:
    void slt_Run ( int index) -
    void slt_OnDone (Task task) - метод-слот, реагирущий на сигнал о
                                  выполнении задачи
    void slt_taskDone(int index) - метод-слот, устанавливающий задачу с
                                   индексом index в состояние "DONE".
===========================================================================*/

class TaskManager : public QObject
{
    Q_OBJECT
public:
    explicit TaskManager ( QObject *parent = 0 );
    TaskManager ( QByteArray data, QObject *parent = 0 );
    ~TaskManager();

    void                addTask (Task &task);
    void                removeTask (int index);
    void                removeTask (Task task);
    void                clear ();
    void                print ();
    void                run ();

private:
    QList <Task>        taskList;
    Scheduler           scheduler;
    int                 limit;

    void                updateTask (int index, int status = 0);
    void                parse ( QByteArray data );
    void                handleTask();
    void                setConfig();

signals:

public slots:
    void                slt_Run ( int index);
    void                slt_OnDone (Task task);
    void                slt_taskDone(int index);
};

#endif // TASKMANAGER_H
