#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QtSql>


enum status_e
{
    NEW_TASK = 1,
    PROCESSING = 2,
    DONE = 3
};

/*======================================================
Класс Task описывает задачу и все возможные операции,
совершаемые над ней.
Функции класса:
    -описание задачи как объекта класса;
    -запись в базу данных информации о задаче.

атрибуты класса:
    cronjob - строка cron-выражения;
    deviceAddress - адрес устройства;
    deviceType - тип устройства;
    login - логин;
    password - пароль;
    task - задание;
    taskArguments - аргументы задания
    priority - приоритет задания;
    status - статус задания;
    id - идентификатор задачи в таблице БД.

методы класса:
    setStatus - метод устанавливает статус задачи;
    setPriority - метод устанавливает приоритет задачи;
    setCronjob - метод устанавливает cron-выражение для
                 задачи;
    setDevAddress - метод устанавливает адрес
                    устройства;
    setDevType - метод устанавливает тип устройства;
    setLogin - метод устанавливает логин для подклю-
               чения к устройству;
    setPassword - метод устанавливает пароль для под-
                  ключения к устройству;
    setTask - метод устанавливает задачу;
    setArguments - метод устанавливает аргументы задачи;
    getStatus - метод возвращает статус задачи;
    getPriority - метод возвращает приоритет задачи;
    removeTask - метод удаляет задачу из БД;
    writeTask - метод записывает задачу в БД;
    editTask - метод редактирует задачу в БД;
    swap - метод копирует аргументы одного объекта
           класса Task в другой объект класса Task.

методы, осуществляющие перегрузку операторов:
    operator = - перегрузка оператора присваивания
    operator == - перегрузка оператора сравнения (==)
    operator != - перегрузка оператора сравнения (!=)
======================================================*/

class Task : public QObject
{
    Q_OBJECT
public:
    explicit Task(const QSqlDatabase & db, QObject *parent = 0 );
    Task ( Task const & obj, QObject *parent = 0 );
    ~Task();

    void            setStatus ( int st );
    void            setPriority ( int pr );
    void            setCronjob ( QString cron );
    void            setDevAddress ( QString devAddr );
    void            setDevType ( QString devType );
    void            setLogin ( QString lgn );
    void            setPassword ( QString pass );
    void            setTask ( QString tsk );
    void            setArgums (QStringList args );
    void            setID( int baseID );
    void            removeTask ();
    void            writeTask ();
    void            editTask (int st = 0, int pr = 0);

    int             getStatus ();
    int             getPriority ();

    QString         getCronjob ();  //--
    QString         getTask ();     //--
    QString         getDevType();   //--

    void            print ();   //заглушка

    Task &          operator = (Task const & obj );
    bool            operator == (const Task & right);
    bool            operator != (const Task & right);

private:
    QSqlDatabase    database;
    QString         cronjob,
                    deviceAddress,
                    deviceType,
                    login,
                    password,
                    task;
    QStringList     taskArguments;
    int             priority,
                    status,
                    id;

    void        swap ( Task & obj );
    void        setIdFromBase();    //-
    QSqlQuery * bindValues(QSqlQuery * & query );    //-
    QString     toString ( QStringList list );    //-

signals:
    void        sig_done( Task );

public slots:
};

#endif // TASK_H
