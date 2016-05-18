#include "HTTPServer.h"

HTTPServer::HTTPServer(QObject *parent) : QObject(parent), server(this)
{
    manager = new QNetworkAccessManager(this);
}

void HTTPServer::start()
{
    if(server.listen(QHostAddress::Any, 8080))
    {
        qDebug() << "[NetworkManager:]\tServer is started\n"
                    "[NetworkManager:]\tListening...";
        connect(&server, SIGNAL(newConnection()), this, SLOT(slotConnectionHandler()));
    }
    else
    {
        qDebug() << "[NetworkManager:]\tError" << server.errorString();
    }
}

void HTTPServer::slt_tConnectionHandler()
{
    qDebug() << "[HTTPServer:]\tHave a new connection\n";
    QTcpSocket * socket = server.nextPendingConnection();
    socket->waitForReadyRead();
    QByteArray request(socket->readAll());
    QStringList buf = QString(request).split(' ');

    if(buf.at(0) == "GET")
    {
        //выполнение задачи, которая пришла в параметрах
        //здесь приходят таски, не созраняющиеся в БД (синглшоты)
        //крона не будет
        //УРАА!!
        /*QString responce = "HTTP/1.1 200 OK\r\n\r\n%1";
        socket->write(responce.arg(QTime::currentTime().toString()).toLatin1());
        socket->waitForBytesWritten();
        socket->disconnectFromHost();
        socket->deleteLater();*/
        return;
    }
    if(buf.at(0) == "POST")
    {
        //добавление строки в БД
        /*QString destUrl(buf.at(1));
        sock->waitForReadyRead();
        QByteArray data(sock->readAll());
        //qDebug() << "put " << QString(data) << " into " << destUrl;
        QString responce = "HTTP/1.1 200 OK\r\n\r\n";
        sock->write(responce.toLatin1());
        sock->waitForBytesWritten();
        sock->disconnectFromHost();
        sock->deleteLater();

        QByteArray payload;
        payload.append(QString(data));
        //payload = QByteArray::fromBase64(QByteArray::fromPercentEncoding(payload));
        //qDebug() << "===========================================\n" << payload << "\n============================================";

        QJsonDocument doc = QJsonDocument::fromJson(payload);
        QJsonObject jObj = doc.object();
        QString table = destUrl.split('/').at(2);
        //qDebug() << "table: " << table << " object: " << jObj;
        emit signalNewJsonObject(table, jObj);*/
        return;
    }
    if(buf.at(0) == "PUT")
    {
        //изменение строки в БД
        /*sock->waitForReadyRead();
        QByteArray data(sock->readAll());

        QString responce = "HTTP/1.1 200 OK\r\n\r\n";
        sock->write(responce.toLatin1());
        sock->waitForBytesWritten();
        sock->disconnectFromHost();
        sock->deleteLater();

        //qDebug() << "PUT";
        QString url = buf.at(1);
        //qDebug() << url;
        QStringList values = url.split('/');
        //qDebug() << values.at(1) << " " << values.at(2);
        QJsonObject jObj = QJsonDocument::fromJson(data).object();
        //qDebug() << jObj;
        emit signalUpdateRecord(values.at(1), jObj, values.at(2));*/
        return;
    }
    if(buf.at(0) == "DELETE")
    {
        //удаление строки из БД
        /*QString responce = "HTTP/1.1 200 OK\r\n\r\n%1";
        sock->write(responce.arg(QTime::currentTime().toString()).toLatin1());
        sock->waitForBytesWritten();
        sock->disconnectFromHost();
        sock->deleteLater();

        //qDebug() << "DELETE";
        QString url = buf.at(1);
        //qDebug() << url;
        QStringList values = url.split('/');
        //qDebug() << values.at(1) << " " << values.at(2);
        emit signalDeleteRecord(values.at(1), values.at(2));*/
        return;
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

