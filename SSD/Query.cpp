#include "query.h"

Query::Query(TableSchema *tableSchema, QObject *parent) : QObject(parent)
{
    schema = tableSchema;
    clear();
    queryModel = new Model();
    connectToDB();
}

Query::Query(Query const &obj, QObject *parent) : QObject(parent)
{
    copy(obj);
}

void Query::copy(const Query &obj)
{
    database = obj.database;
    schema = obj.schema;
    selectedFields = obj.selectedFields;
    selectedList = obj.selectedList;
    where = obj.where;
    limit = obj.limit;
    order = obj.order;
    sqlString = obj.sqlString;
    countString = obj.countString;
    innerJoin = obj.innerJoin;
    rName = obj.rName;
    queryModel = obj.queryModel;
}

Query & Query::operator = (Query const & obj)
{
    copy(obj);
    return *this;
}

bool Query::operator == ( const Query &right )
{
    if ( ( schema == right.schema ) &&
         ( selectedFields == right.selectedFields ) &&
         ( selectedList == right.selectedList) &&
         ( schema == right.schema ) &&
         ( where == right.where ) &&
         ( limit == right.limit ) &&
         ( order == right.order ) &&
         ( sqlString == right.sqlString ) &&
         ( countString == right.countString ) &&
         ( innerJoin == right.innerJoin ) &&
         ( rName == right.rName )   &&
         ( queryModel == right.queryModel))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Query::operator != ( const Query & right )
{
    return !( this == & right );
}

void Query::clear()
{
    selectedFields = "*";
    where = "";
    limit = "";
    order = "";
    sqlString = "";
    countString = "";
    innerJoin = "";
    rName = "";
}

void Query::setWhere(QString fieldName, QVariant value)
{
    where = " WHERE ";
    where += fieldName + " = ";
    if(value.type() == QVariant::String)
    {
        where += "'" + value.toString() + "'";
    }
    else
    {
        where += value.toString() ;
    }
}

void Query::setOrder(QString fieldName)
{
    order = " ORDER BY " + fieldName;
}

void Query::setSelectedFields(QStringList fields)
{
    selectedFields = listToString(fields);
    selectedList = fields;
}

void Query::setLimit(QString str)
{
    limit = " LIMIT " + str;
}

void Query::withOuter(QString relationName)
{
    rName = relationName;
    relation_s r = schema->getRelation(rName);
    try
    {
        if((r.link.first != "") && (r.link.second != "") && (r.type != 0))
        {
            TableSchema *sch = r.model->getSchema();
            outerJoin =  " LEFT OUTER JOIN ";
            outerJoin += sch->getTableName() + " ON ";
            outerJoin += schema->getRelation(rName).link.first;
            outerJoin += " = " + schema->getRelation(rName).link.second;
        }
        else
        {
            throw r;
        }
    }
    catch(relation_s)
    {
        qDebug() << "[Query::WithOuter]:\tError! Can not get relation" << rName << "!";
        qDebug() << r.link.first << "; " << r.link.second << "; " << r.type;
    }
}

void Query::withInner(QString relationName)
{
    rName = relationName;
    relation_s r = schema->getRelation(rName);
    try
    {
        if((r.link.first != "") && (r.link.second != "") && (r.type != 0))
        {
            TableSchema *sch = r.model->getSchema();
            innerJoin =  " INNER JOIN ";
            innerJoin += sch->getTableName() + " ON ";
            innerJoin += schema->getRelation(rName).link.first;
            innerJoin += " = " + schema->getRelation(rName).link.second;
        }
        else
        {
            throw r;
        }
    }
    catch(relation_s)
    {
        qDebug() << "[Query::WithInner]:\tError! Can not get relation" << rName << "!";
    }
}

QString Query::generateSQL()
{
    QString sql = "SELECT ";
    if(countString != "")
    {
        sql += countString;
    }
    else
    {
        sql += selectedFields + " ";
    }
    sql += " FROM ";
    sql += schema->getTableName();
    if(outerJoin != "")
    {
        sql += outerJoin;
    }
    else if(innerJoin != "")
    {
        sql += innerJoin;
    }
    sql += where;
    sql += order;
    sql += limit;
    return sql;
}

int Query::count(QString fieldName)
{
    countString = "COUNT(" + fieldName + ")";
    QString sql = generateSQL();

    list_t list = requestOne(sql);
    if(!list.empty())
    {
        return list[0].second.toInt();
    }
    else
    {
        return 0;
    }
}

Model * Query::getOne()
{    
    sqlString = generateSQL();

    list_t vector = requestOne(sqlString);
    try
    {
        if(vector.empty())
        {
            throw vector;
        }
        if((innerJoin == "") && (outerJoin == ""))
        {
            queryModel->setSchema(schema);
            queryModel = appendRecords(vector, queryModel);
        }
        else
        {
            Model * temp = schema->getRelation(rName).model;
            relation_data_t data = getRelationModel(vector, temp);
            schema->setRelationData(data);
            queryModel->setSchema(schema);
            queryModel = appendRecords(vector, queryModel);
        }
    }
    catch(list_t)
    {
        qDebug() << "[Query::getOne]:\tRequest list is empty!";
    }
    return queryModel;
}

QList<Model *> Query::getAll()
{
    sqlString = generateSQL();
    matrix_t matrix = requestMany(sqlString);

    QList <Model *> list;
    int i=0;
    while(i != matrix.size())
    {
        Model * model = new Model();
        model->setSchema(schema);
        model = appendRecords(matrix[i], model);
        list.append(model);
        i++;
    }
    return list;
}

Model * Query::getEmpty()
{
    Model * model = new Model();
    model->setSchema(schema);
    return model;
}

void Query::updateSchema(Model *model)
{
    schema = model->getSchema();
    queryModel = model;
}

Model * Query::appendRecords(list_t values, Model * & model)
{
    if(!values.empty())
    {
        model->setIsNew(false);
        QStringList fields = model->getSchema()->getFields();
        for(int i = 0; i < values.size(); i++)
        {
            if(fields.contains(values[i].first))
            {
                model->setField(values[i].first, values[i].second);
            }
        }
    }    
    return model;    
}

relation_data_t Query::getRelationModel(list_t values, Model * & model)
{
    relation_data_t rData;
    QList <Model *> list;
    if(!values.empty())
    {
        QStringList fields = model->getSchema()->getFields();
        int i = 0;
        while(i < values.size())
        {
            int j = i;
            Model * temp = new Model();
            temp->setSchema(model->getSchema());
            temp->setIsNew(false);
            while(values[j] != QPair <QString, QVariant> ("", ""))
            {
                if((fields.contains(values[j].first)) && (values[j].second != ""))
                {
                    temp->setField(values[j].first, values[j].second);
                }                
                j++;
            }
            list.append(temp);
            i = j + 1;
        }
    }
    rData.insert(rName, list);
    return rData;
}

list_t Query::requestOne(QString sql)
{
    list_t result;
    QSqlQuery *query = new QSqlQuery(database);
    query->prepare(sql);
    try
    {
        if(query->exec())
        {
            while(query->next())
            {
                for(int i = 0; i < query->record().count(); i++)
                {
                    QPair <QString, QVariant> pair(query->record().fieldName(i), query->value(i));
                    if((innerJoin == "") && (outerJoin == ""))
                    {
                        result.append(pair);
                    }
                    else
                    {
                        QSqlRecord records = database.record(schema->getRelation(rName).model->getSchema()->getTableName());
                        if(records.contains(query->record().fieldName(i)))
                        {
                            result.append(pair);
                        }
                    }
                }
                result.append(QPair <QString, QVariant> ("", ""));
            }
        }
        else
        {
            throw query;
        }
    }
    catch (QSqlQuery *)
    {
        qDebug() << "[Query error]: " << query->lastError();
    }

    return result;
}

matrix_t Query::requestMany(QString sql)
{
    matrix_t result;
    QSqlQuery *query = new QSqlQuery(database);
    query->prepare(sql);
    try
    {
        if(query->exec())
        {
            while(query->next())
            {
                QList <QPair <QString, QVariant>> list;
                for(int i = 0; i < schema->getFieldsCount(); i++)
                {
                    QPair <QString, QVariant> pair(query->record().fieldName(i), query->value(i));
                    list.append(pair);
                }
                result.append(list);
            }
        }
        else
        {
            throw query;
        }
    }
    catch (QSqlQuery *)
    {
        qDebug() << "[Query error]: " << query->lastError();
    }
    return result;
}

void Query::connectToDB()
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

QVariant Query::getlastID()
{
    selectedFields = "last_insert_rowid()";
    QString sql = generateSQL();
    list_t list = requestOne(sql);
    if(!list.empty())
    {
        return list[0].second;
    }
    else
    {
        return 0;
    }
}

QString Query::listToString(QStringList list)
{
    QString result = "";
    QString temp = "";
    int i=0;
    while(i != list.size())
    {
        result += temp + list[i];
        temp = ", ";
        i++;
    }
    return result;
}

Query::~Query()
{

}

