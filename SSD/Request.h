#ifndef REQUEST_H
#define REQUEST_H

#include <QObject>
#include <QString>

class Request : public QObject
{
    Q_OBJECT
public:
    explicit Request(QString tableName = "", QObject *parent = 0);
    ~Request();

    bool post(QJsonDocument document);
    QJsonDocument get();
    void setTablename(QString tableName);
private:
    QString tabName,
            url,
            part1,
            part2;

    QString getTableName(QString tableName);
    QString getUrl();
signals:

public slots:
};

#endif // REQUEST_H
