#ifndef CRONPARSER_H
#define CRONPARSER_H

#include <QObject>
#include <QString>
#include <QPair>
#include <QVector>
#include <QDateTime>

 /*==========================================================================================
  Класс CronParser выполняет следующие задачи:
    - обработка строки, содержащей cron-выражение задачи
    - вычисление ближайшей даты вызова текущей задачи

  Атрибуты класса:
  1) cronJob   - строка, содержащая cron-выражение;
  2) cronDate  - дата вызова функции;
  3) minute, hour, dayOfMonth, month,
     year, dayOfWeek - пары значений, содержащие информацию об отдельной части
                       даты и времени (минута, час, день месяца, месяц, год,
                       день недели соответственно). Первый элемент пары -
                       значение части даты и времени, второе значение - флаг
                       переполнения разряда текущей части даты и времени;
  4) isCalled   - флаг, показывающий, что объект данного класса уже был вызван.

  Методы класса:
  1) CronParser(QObject *parent = 0)                        - конструктор класса по умолчанию;
  2) CronParser ( CronParser & obj, QObject * parent = 0 )  - конструктор копирования;
  3) ~CronParser() - деструктор класса
  4) QString getCronJob()                                   - метод возвращает текущее
                                                              cron-выражение;
  5) QDateTime getDateTime(QString cron)                    - метод возвращает время ближайшего
                                                              вызова функции;
  6) QPair<int, bool> parse
    (QString cronJob, int var, int minLimit, int maxLimit)  - метод обрабатывает cron-выражение
                                                              и возвращает значение
                                                              срабатывания для каждой единицы
                                                              времени;
  7) QDateTime calcTaskDate()                               - метод парсит cron-выражение и
                                                              рассчитывает ближайшую дату
                                                              вызова функции;
  8) void isSingle(bool singleShot)                         - сигнал, оповещающий о том, что
                                                              данная задача является
                                                              синглшотом
  ========================================================================================== */
class CronParser : public QObject
{
    Q_OBJECT

public:
    explicit CronParser(QObject *parent = 0);
    CronParser ( CronParser & obj, QObject * parent = 0 );
    ~CronParser();

    QString getCronJob();
    QDateTime getDateTime(QString cron);
    void setCall(bool var);
    CronParser &        operator = ( CronParser const & obj );

private:
    QString cronJob;
    QDateTime cronDate;
    QPair<int, bool> minute,
                     hour,
                     dayOfMonth,
                     month,
                     year,
                     dayOfWeek;
    bool isCalled;

    QPair<int, bool> parse(QString cronJob, int var, int minLimit, int maxLimit);
    QDateTime calcTaskDate();
signals:
    void isSingle(bool singleShot);

public slots:
};

#endif // CRONPARSER_H
