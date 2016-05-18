#include "Request.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>

Request::Request(QString requestUrl, QObject *parent) : QObject(parent)
{
    url = requestUrl;
}

void Request::post()
{

}

void Request::get()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
    QEventLoop   eLoop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &eLoop, SLOT(quit()));
    try
    {
        eLoop.exec();
        if(reply->error() != QNetworkReply::NoError)
        {
            throw reply;
        }
        else
        {
            QString json = reply->readAll();
            QJsonDocument parser = QJsonDocument::fromJson(json.toUtf8());
            qDebug() << parser;
        }
    }
    catch(QNetworkReply *)
    {
        qDebug() << "[!]ERROR:\t" << reply->errorString();
    }
}

Request::~Request()
{

}

