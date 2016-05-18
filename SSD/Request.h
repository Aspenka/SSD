#ifndef REQUEST_H
#define REQUEST_H

#include <QObject>
#include <QString>

class Request : public QObject
{
    Q_OBJECT
public:
    explicit Request(QString requestUrl, QObject *parent = 0);
    ~Request();

    void post();
    void get();
private:
    QString url;
signals:

public slots:
};

#endif // REQUEST_H
