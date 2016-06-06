#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonObject>
#include <QJsonArray>
#include <QTcpServer>
#include <QTcpSocket>

class HTTPServer : public QObject
{
    Q_OBJECT
public:
    explicit HTTPServer(QObject *parent = 0);
    ~HTTPServer();
    void start();
private:
    QNetworkAccessManager*  manager;
    QTcpServer              server;

    QJsonDocument toJsonDocument(QString command);

signals:

public slots:
    void                    slt_ConnectionHandler();

};

#endif // HTTPSERVER_H
