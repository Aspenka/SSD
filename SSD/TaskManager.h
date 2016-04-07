#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>
#include <QList>
#include <QtSql>
#include <QVector>
#include <QPair>
#include <QVector>
#include <QSettings>        //заглушка
#include "Task.h"
#include "Scheduler.h"

/*======================================================
 Класс TaskManager предназначен для управления задачами,
 поступающими из Центрального сервера.
 Основные функции класса:
    -обработка пакета задач, поступающего из ЦС;
    -формирование списка команд для УСПД;
    -управление сформированным списком команд;
    -составление расписания выполнения команд;
    -вызов команд в соответствии с расписанием.

 атрибуты класса:
    tasks - список задач;
    sheduler - объект планировщика задач;
    database - объект для подключения к базе данных

 методы класса:
    addTask - метод добавляет новую задачу в список;
    removeTask  - метод удаляет задачу из списка по
                  индексу;
    removeTask - метод удаляет задачу из списка по
                 содержимому;
    clear - метод очщает перечень задач;
    handleTask - метод запускает задачу в обработку;   

сигналы класса:
    signalDone - сигнал оповещает о том, что задача
                 выполнена;

слоты класса:
    slotRun - слот запускает задачу на выполнение
======================================================*/

#define taskVector_t      QVector <QPair <QString, int>>           //вектор заданий формата TaskPair

class TaskManager : public QObject
{
    Q_OBJECT
public:
    explicit TaskManager ( QObject *parent = 0 );
    TaskManager ( QByteArray data, QObject *parent = 0 ); //заглушка
    ~TaskManager();

    void            addTask (Task &task, int priority = 0);
    void            removeTask ( int index );
    void            removeTask ( Task task );
    void            clear ();
    void            handleTask (taskVector_t vector);
    void            print ();       //заглушка
    void            run ();         //заглушка

    //void            updateTask ( int index, int status = 0, int priority = 0 ); //--

private:
    QList <Task>    taskList;
    Scheduler       scheduler;
    QSqlDatabase    database;

    void            updateTask ( int index, int status = 0, int priority = 0 );
    void            parse ( QByteArray data );
    void            sortTasklist (QList<Task> &list, int left, int right);
    void            parse ();           //заглушка
    void            checkTaskTable();   //--
    void            clearTaskTable();   //--
    taskVector_t    toTaskVector();     //--

signals:

public slots:
    void            slt_Run ( int index );
    void            slt_OnDone (Task task);  //--
};

#endif // TASKMANAGER_H
