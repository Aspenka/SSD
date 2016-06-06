#include "model.h"
#include <QDebug>

Model::Model(QObject *parent) : QObject(parent)
{

}

Model::Model(const Model &obj, QObject *parent) : QObject(parent)
{
    copy(obj);
}

void Model::copy(const Model & obj)
{
    record = obj.record;
    isNew = obj.isNew;
    schema = obj.schema;
    database = obj.database;
    relation = obj.relation;
}

Model & Model::operator =(Model const & obj)
{
    copy(obj);
    return *this;
}

Model * Model::operator =(Model const * obj)
{
    copy(*obj);
    return this;
}

bool Model::operator == ( const Model &right )
{
    if ( ( record == right.record ) &&
         ( isNew == right.isNew ) &&
         ( schema == right.schema ) &&
         ( relation == right.relation))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Model::operator != ( const Model & right )
{
    return !( this == & right );
}

bool Model::getIsNew()
{
    return isNew;
}

QStringList Model::getFields()
{
    return schema->getFields();
}

QStringList Model::getSelectedFields(QStringList fieldList)
{
    QStringList result;
    for(int i = 0; i < fieldList.size(); i++)
    {
        result.append(schema->getTableName()+ "." + fieldList[i]);
    }

    return result;
}

QList <Model*> Model::getRelationData(QString relationName)
{
    relation_data_t data = schema->getRelationData();
    QList <Model*> list = data.value(relationName);
    return list;
}

void Model::setField(QString fieldName, QVariant value)
{
    try
    {
        if(schema->checkField(fieldName))
        {
            record.insert(fieldName, value);
        }
        else
        {
            throw fieldName;
        }
    }
    catch(QString)
    {
        qDebug() << fieldName << "is not a member of " << schema->getTableName();
    }
}

QVariant Model::getRecord(QString fieldName)
{
    QVariant result;
        try
        {
            if(schema->checkField(fieldName))
            {
                result = record.value(fieldName);
            }
            else
            {
                throw fieldName;
            }
        }
        catch(QString)
        {
            qDebug() << "[Model::getRecord]:\tError! Field " << fieldName
                     << " does not exost in table " << schema->getTableName();
        }
        return result;
}

void Model::setSchema(TableSchema *tableSschema)
{
    schema = tableSschema;
}

void Model::setIsNew(bool value)
{
    isNew = value;
}

void Model::setRelationData(QString relationName, QList <Model *> model)
{
    relation_data_t data = {{relationName, model}};
    schema->setRelationData(data);
}

bool Model::save(QStringList fields)
{
    if(getIsNew() == true)
    {
        bool result = insert();
        if(result == true)
        {
            setIsNew(false);
        }
        return result;
    }
    else
    {
        return update(fields);
    }
}

bool Model::remove()
{
    QString sql = "DELETE FROM " + schema->getTableName();
    sql += " WHERE ";
    QList <QString> pk = schema->getPrimaryKey();
    QMap <QString, QVariant>::iterator i = record.begin();
    QString temp = "";
    while(i != record.end())
    {
        for(int j = 0; j < pk.size(); j++)
        {
            if(i.key() == pk[j])
            {
                sql += temp + i.key() + " = ";
                sql += getString(i.value());
                temp = " AND ";
            }
        }
        i++;
    }
    return execQuery(sql);
}

bool Model::insert()
{
    QString sql = "INSERT INTO " + schema->getTableName() + " (";
    QString fields = "";
    QString values = "";
    QMap <QString, QVariant>::iterator i = record.begin();
    QString temp = "";
    while(i != record.end())
    {
        fields += temp + i.key();
        values += temp + getString(i.value());
        temp = ", ";
        i++;
    }
    sql += fields + ") VALUES (";
    sql += values + ")";
    return execQuery(sql);
}

bool Model::update(QStringList fields)
{
    QString sql = "UPDATE " + schema->getTableName();
    sql += " SET ";
    QMap <QString, QVariant>::iterator i = record.begin();
    QString temp = "";
    while(i != record.end())
    {
        if((fields.empty()) || (fields.contains(i.key())))
        {
            sql += temp + i.key() + "=";
            sql += getString(i.value());
            temp = ", ";
        }
        i++;
    }
    sql += " WHERE ";
    temp = "";
    int j=0;
    while( j != schema->getPrimaryKey().size())
    {
        sql += temp + schema->getPrimaryKey()[j] + " = ";
        sql += record.value(schema->getPrimaryKey()[j]).toString();
        temp = " AND ";
        j++;
    }
    return execQuery(sql);
}

bool Model::execQuery(QString sql)
{
    connectToDB();
    QSqlQuery *query = new QSqlQuery(database);
    query->prepare(sql);
    try
    {
        if(query->exec())
        {
            return true;
        }
        else
        {
            throw query;
        }
    }
    catch (QSqlQuery *)
    {
        qDebug() << "[Query error]: " << query->lastError();
        return false;
    }
}

void Model::connectToDB()
{
    if(QSqlDatabase::contains("dbConnection"))
    {
        database = QSqlDatabase::database("dbConnection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE", "dbConnection");
        database.setDatabaseName("base.db");
    }
    try
    {
        if(!database.open())
        {
            throw database;
        }
    }
    catch(QSqlDatabase)
    {
        qDebug() << "[DB Connecting]:    Database connecting error! " << database.lastError();
    }
}

QString Model::getString(QVariant value)
{
    QString res;
    if(value.type() == QVariant::String)
    {
        res = "'" + value.toString() + "'";
    }
    else
    {
        res = value.toString();
    }
    return res;
}

TableSchema *Model::getSchema()
{
    return schema;
}


void Model::setRelation(Model *outModel, QString relationName)
{
    relation.model = outModel;
    schema->setRelations(relationName, relation);
}

//--
void Model::print()
{
    QMap <QString, QVariant>::iterator i;
    for(i = record.begin(); i != record.end(); i++)
    {
        qDebug() << i.key() << ":\t" << i.value().toString();
    }
}

//--
void Model::printRelation()
{
    relation_data_t data = schema->getRelationData();
    QMap <QString,QList< Model *>>::iterator i = data.begin();
    qDebug() << "============== RELATIONS ===============";
    while(i != data.end())
    {
        QList <Model *> list = i.value();
        for(int j = 0; j < list.size(); j++)
        {
            qDebug() << i.key() << ":";
            list[j]->print();
        }
        i++;
    }
}

Model::~Model()
{

}

