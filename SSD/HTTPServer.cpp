#include "HTTPServer.h"
#include "Message.h"
#include "Request.h"


HTTPServer::HTTPServer(QObject *parent) : QObject(parent), server(this)
{
    manager = new QNetworkAccessManager(this);
}

void HTTPServer::start()
{
    if(server.listen(QHostAddress::Any, 8081))
    {
        qDebug() << "[HTTPServer:]\tHTTP Server is started\n"
                    "[HTTPServer:]\tListening...";
        connect(&server, SIGNAL(newConnection()), this, SLOT(slt_ConnectionHandler()));

        Request * r = new Request();
        QJsonDocument data = toJsonDocument("get_data");
        r->post(data);
    }
    else
    {
        qDebug() << "[NetworkManager:]\tError" << server.errorString();
    }
}

void HTTPServer::slt_ConnectionHandler()
{
    qDebug() << "[HTTPServer:]\tHave a new connection\n";
    QTcpSocket * socket = server.nextPendingConnection();
    socket->waitForReadyRead();
    QByteArray request(socket->readAll());
    QStringList buf = QString(request).split(' ');

    qDebug() << "Data received:\t" << buf.at(0);

    if(buf.at(0) == "POST")
    {
        QString responce = "HTTP/1.1 200 OK\r\n\r\n%1";
        socket->waitForReadyRead();
        QByteArray data(socket->readAll());
        socket->write(responce.toLatin1());
        socket->waitForBytesWritten();
        socket->disconnectFromHost();
        socket->deleteLater();

        qDebug() << "Data received: " << data;
        //--------
        //QJsonDocument doc = QJsonDocument::fromJson(data);
        //parse(doc);
    }
}

QJsonDocument HTTPServer::toJsonDocument(QString command)
{
    QJsonObject json;
    json.insert("request", QJsonValue(command));
    QJsonDocument document(json);
    return document;
}

HTTPServer::~HTTPServer()
{

}

