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


/*=================================================================
 Класс Task описывает задачу и предоставляет методы для работы
 в ней.

 Атрибуты класса:
 1) cronjob - строка, содержащая cron-выражение задачи;
 2) parameters - параметры, необходимые для выполнения задачи;
 3) status - статус выполнения задачи;
 4) uid - идентификационный номер задачи в ВБД;
 5) model - модель таблицы задач в ВБД;
 6) query - объект, предназначенный для запросов на выборку из ВБД;
 7) deviceUid - идентификатор устройства, для которого
                предназначена задача.

 Методы класса:
 1) Task() - конструктор по умолчанию;
 2) Task ( Task const & obj) - конструктор копирования;
 3) ~Task() - длеструктор;
 4) void setStatus(int st) - метод устанавливает статус задачи;
 5) void setUid(int i) - метод устанавливает уникальный
                         идентификатор задачи;
 6) void setCronjob(QString cron) - метод устанавливает cron-
                                    выражение для задачи;
 7) void setParameters(QString params) - метод устанавливает
                                         параметры для выполнения
                                         задачи;
 8) void setDeviceUid(QString uid) - метод устанавливает значение
                                     идентификатора УСПД;
 9) int getUid() - метод возвращает уникальный идентификатор
                   задачи из ВБД;
 10) QList <Task> getTaskList() - метод формирует и возвращает
                                 список задач;
 11) QString getCronjob() - метод возвращает текущее
                            cron-выражение задачи;
 12) QString getParameters() - метод возвращает текущие параметры
                               задачи;
 13) QString getDeviceUid() -  метод возвращает идентификатор УСПД,
                               для которого предназначена задача;
 14) bool isEmpty() - метод проверяет, пуста ли таблица задач в
                      ВБД. Если таблица пуста, мтеод возвращает
                      true, в противном случае метод возвращает
                      false;
 15) void save() - метод сохраняет текущую задачу в ВБД;
 16) void remove() - метод удаляет текущую задачу из ВБД;
 17) void edit(int taskStatus) - метод редактирует информацию о
                                 статусе задачи в ВБД;
 18) void print() - метод выводит информацию о текущей
                    задаче в консоль приложения;
 19) Task & operator = (Task const & obj) - метод перегружает
                                            операцию присваивания;
 20) bool operator ==
    (const Task & right) - метод перегружает операцию сравнения.
                           Если один объект данного класса равен
                           другому объекту этого же класса, метод
                           возвращает true, в противном случае
                           метод возвращает false;
 21) bool operator !=
     (const Task & right) - метод перегружает операцию сравнения.
                           Если один объект данного класса не
                           равен другому объекту этого же класса,
                           метод возвращает true, в противном
                           случае метод возвращает false ;
 22) void copy(const Task &obj) - метод копирует данные из объекта
                                  obj в текущий объект;
 23) void sig_done(Task) - сигнал, оповещающий о том, что задача
                           выполнена.
 ================================================================*/
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
    void            setDeviceUid(QString uid);

    int             getUid();
    QList <Task>    getTaskList();
    QString         getCronjob();
    QString         getParameters();
    QString         getDeviceUid();

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
                    parameters,
                    deviceUid;
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
