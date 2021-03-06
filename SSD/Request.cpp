#include "Request.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariant>

Request::Request(QString tableName, QObject *parent) : QObject(parent)
{
    if(tableName != "")
    {
        part1 = "http://pvo.ascae.ru/api/sync/get?represent=api/";
        part2 = "/get-dcs&access-token=api_token";
        tabName = getTableName(tableName);
    }
}

QString Request::getUrl()
{
    url = part1 + tabName + part2;
    return url;
}

void Request::setTablename(QString tableName)
{
    url.clear();
    tabName = getTableName(tableName);
}

bool Request::post(QJsonDocument document)
{
    QByteArray data = document.toJson();

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QEventLoop eLoop;
    QNetworkRequest request(QUrl("http://localhost:8080/"));
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.count());

    QNetworkReply *reply = manager->post(request, data);
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
            qDebug() << "[REQUEST]:\tSent request";
            return true;
        }
    }
    catch(QNetworkReply *)
    {
        qDebug() << "[!][REQUEST]:\tERROR! " << reply->errorString();
        return false;
    }
}

QJsonDocument Request::get()
{
    url = getUrl();
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
    QEventLoop   eLoop;
    QJsonDocument parser;
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
            parser = QJsonDocument::fromJson(json.toUtf8());
        }
    }
    catch(QNetworkReply *)
    {
        qDebug() << "[!][REQUEST]:\tERROR! " << reply->errorString();
    }
    return parser;
}

QString Request::getTableName(QString tableName)
{
    QString name = "";
    if(tableName == "device")
    {
        name = tableName;
    }
    else
    {
        if(tableName == "net_address")
        {
            name = "net-address";
        }
        else
        {
            if(tableName == "net_address_type")
            {
                name = "net-address-type";
            }
        }
    }
    try
    {
        if(name == "")
        {
            throw name;
        }
    }
    catch(QString)
    {
        qDebug() << "[!][REQUEST]:\tERROR! Table name is incorrect\n";
    }
    return name;
}

Request::~Request()
{

}

