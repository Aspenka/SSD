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
        qDebug() << "[]\tHTTP Server is started\n"
                    "[]\tListening...";
        connect(&server, SIGNAL(newConnection()), this, SLOT(slt_ConnectionHandler()));

        Income * income = new Income();
        Request * r = new Request();
        r->post("get_data");
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
        /*socket->write(responce.arg(QTime::currentTime().toString()).toLatin1());
        socket->waitForBytesWritten();
        socket->disconnectFromHost();
        socket->deleteLater();*/
        //добавление строки в БД
        QString destUrl(buf.at(1));
        socket->waitForReadyRead();
        QByteArray data(socket->readAll());
        //qDebug() << "put " << QString(data) << " into " << destUrl;
        //QString responce = "HTTP/1.1 200 OK\r\n\r\n";
        socket->write(responce.toLatin1());
        socket->waitForBytesWritten();
        socket->disconnectFromHost();
        socket->deleteLater();

        QByteArray payload;
        payload.append(QString(data));
        //payload = QByteArray::fromBase64(QByteArray::fromPercentEncoding(payload));
        //qDebug() << "===========================================\n" << payload << "\n============================================";

        QJsonDocument doc = QJsonDocument::fromJson(payload);
        QJsonObject jObj = doc.object();
        QString table = destUrl.split('/').at(2);
        qDebug() << "table: " << table << " object: " << jObj;
        //emit signalNewJsonObject(table, jObj);
        //return;
    }
}

/*void HTTPServer::doDownload(const QString tableName, const QByteArray &data)
{
    QString url = QString("http://80.89.147.56/index.php?r=api/sync/get&table=%1&access-token=api_token").arg(table);
    qDebug() << url;
    QNetworkReply *r = m_pQnaManager->get(QNetworkRequest(QUrl(url)));
    QEventLoop eLoop;
    connect(m_pQnaManager, SIGNAL(finished(QNetworkReply*)), &eLoop, SLOT(quit()));
    eLoop.exec();
    if(r->error() != QNetworkReply::NoError)
    {
        qDebug() << " [!]ERROR: " << r->errorString();
    }
    else
    {
        qDebug() << r->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << r->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        QJsonDocument doc = QJsonDocument::fromJson(r->readAll());
        qDebug() << doc;
        if (doc.isObject())
        {
        QJsonObject jObj = doc.object();
        emit signalNewJsonObject(table, jObj);
        }
        else if (doc.isArray())
        {
         QJsonArray jArr = doc.array();
         emit signalNewJsonObject(table, jArr);
        }
    }
    r->deleteLater();
}*/

/*void HTTPServer::doUpload(const QString &table, const QByteArray &data)
{
    QString url = QString("http://80.89.147.56/index.php?r=api/sync/post&access-token=api_token&table=%1").arg(table);
    qDebug() << url;
    QNetworkRequest req;
    req.setUrl(url);
    req.setHeader(QNetworkRequest::ContentLengthHeader, data.count());

    QNetworkReply *r = m_pQnaManager->post(req, data);
    QEventLoop eLoop;
    connect(m_pQnaManager, SIGNAL(finished(QNetworkReply*)), &eLoop, SLOT(quit()));
    eLoop.exec();
    if(r->error() != QNetworkReply::NoError)
    {
        qDebug() << " [!]ERROR: " << r->errorString();
    }
    else
    {
        qDebug() << " [-]Reply:";
        qDebug() << " [|] " << r->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << " [|] " << r->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        qDebug() << " [|] " << r->readAll();
        qDebug() << " [-]END";
    }
    r->deleteLater();
}*/

/*void HTTPServer::slt_SendPart(const QString tableName, QList<QJsonObject> list)
{
    foreach (QJsonObject jObj, list)
    {
      QByteArray data(QJsonDocument(jObj).toJson());
      doUpload(table, data);
    }
}*/

HTTPServer::~HTTPServer()
{

}

