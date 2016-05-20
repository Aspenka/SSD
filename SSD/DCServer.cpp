#include <cstdlib>
#include <QVariant>
#include <QMap>
#include "DCServer.h"
#include "HTTPServer.h"
#include "TableModel.h"
#include "var.h"
#include "Request.h"

DCServer::DCServer ( QObject *parent) : QObject(parent )
{

}

void DCServer::startServer ( )
{
    HTTPServer *server = new HTTPServer();
    server->start();
    //Request * r = new Request("net_address_type");
    //QJsonDocument data = r->get();
    //parse("net_address_type", data);

    /*r->setTablename("net_address");
    data = r->get();
    parse("net_address", data);*/

    /*r->setTablename("device");
    data = r->get();
    parse("device", data);*/

}

void DCServer::parse(QString tableName, QJsonDocument data)
{
    Var v(data);
    QList    <Var> list = v.getList();
    if(tableName == "net_address_type")
    {
        NetAddressTypeModel *model = new NetAddressTypeModel();
        addToTable(list, model);
    }
    else
    {
        if(tableName == "net_address")
        {
            NetAddressModel *model = new NetAddressModel();
            addToTable(list, model);
        }
        else
        {
            if(tableName == "device")
            {
                DeviceModel * model = new DeviceModel();
                addToTable(list, model);
            }
        }
    }
}

template <class T>
void DCServer::addToTable(QList <Var> list, T *&model)
{
    for(int i = 0; i < list.size(); i++)
    {
        model->setIsNew(true);
        QMap <QString, Var> map = list[i].getMap();
        QMap <QString, Var>::iterator it = map.begin();
        while(it != map.end())
        {
            model->setField(it.key(), it.value().toQVar());
            it++;
        }
        if(model->save())
        {
            qDebug() << "[]:\trecord saved in net_address_type";
        }
    }
}

DCServer::~DCServer ( )
{

}

