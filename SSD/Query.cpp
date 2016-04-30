#include "query.h"

Query::Query(TableSchema *tableSchema, QObject *parent) : QObject(parent)
{
    schema = tableSchema;
    clear();
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
    selectedList == obj.selectedList;
    where = obj.where;
    limit = obj.limit;
    order = obj.order;
    sqlString = obj.sqlString;
    countString = obj.countString;
    innerJoin = obj.innerJoin;
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
         ( innerJoin == right.innerJoin ))
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

void Query::withOuter(QString relationName, QString inField, QString outField)
{
    relation_s r = schema->getRelation(relationName);
    try
    {
        if((!r.link.empty()) && (r.type != 0))
        {
            Model *model = r.Model;
            TableSchema *sch = model->getSchema();
            outerJoin =  " LEFT OUTER JOIN ";
            outerJoin += sch->getTableName() + " ON ";
            outerJoin += inField + " = " + outField;
            QString sql = generateSQL();

            TableSchema *temp = generateTableSchema(selectedList, schema, sch);
            matrix_t matrix = requestMany(sql, temp);
            QList <Model *> list;
            int i=0;
            while(i != matrix.size())
            {
                Model * model = new Model();
                model->setSchema(temp);
                model = appendRecords(temp, matrix[i], model);
                model->print();
                qDebug() << "\n";
                list.append(model);
                i++;
            }
        }
        else
        {
            throw r;
        }
    }
    catch(relation_s)
    {
        qDebug() << "[Query::WithOuter]:\tError! Can not get relation" << relationName << "!";
    }
}

void Query::withInner(QString relationName, QString inField, QString outField)
{
    relation_s r = schema->getRelation(relationName);
    try
    {
        if((!r.link.empty()) && (r.type != 0))
        {
            Model *model = r.Model;
            TableSchema *sch = model->getSchema();
            innerJoin =  " INNER JOIN ";
            innerJoin += sch->getTableName() + " ON ";
            innerJoin += inField + " = " + outField;
            QString sql = generateSQL();

            TableSchema *temp = generateTableSchema(selectedList, schema, sch);
            matrix_t matrix = requestMany(sql, temp);
            QList <Model *> list;
            int i=0;
            while(i != matrix.size())
            {
                Model * model = new Model();
                model->setSchema(temp);
                model = appendRecords(temp, matrix[i], model);
                model->print();
                qDebug() << "\n";
                list.append(model);
                i++;
            }
        }
        else
        {
            throw r;
        }
    }
    catch(relation_s)
    {
        qDebug() << "[Query::WithInner]:\tError! Can not get relation" << relationName << "!";
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

    QList <QVariant> list = requestOne(sql, schema);
    if(!list.empty())
    {
        return list[0].toInt();
    }
    else
    {
        return 0;
    }
}

Model * Query::getOne()
{
    sqlString = generateSQL();
    QList <QVariant> vector = requestOne(sqlString, schema);

    Model * model = new Model();
    model->setSchema(schema);
    model = appendRecords(schema, vector, model);
    return model;
}

QList<Model *> Query::getAll()
{
    sqlString = generateSQL();
    matrix_t matrix = requestMany(sqlString, schema);

    QList <Model *> list;
    int i=0;
    while(i != matrix.size())
    {
        Model * model = new Model();
        model->setSchema(schema);
        model = appendRecords(schema, matrix[i], model);
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

Model * Query::appendRecords(TableSchema *tableSchema, QList<QVariant> values, Model *&model)
{
    if(!values.empty())
    {
        model->setIsNew(false);
        for(int i = 0; i < tableSchema->getFieldsCount(); i++)
        {
            model->setField(tableSchema->getFields()[i], values[i]);
        }
    }
    return model;
}

QList<QVariant> Query::requestOne(QString sql)
{
    QList <QVariant> result;
    QSqlQuery *query = new QSqlQuery(database);
    query->prepare(sql);
    try
    {
        if(query->exec())
        {
            while(query->next())
            {
                for(int i = 0; i < tableSchema->getFieldsCount(); i++)
                {
                    result.append(query->value(i));
                }
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

matrix_t Query::requestMany(QString sql, TableSchema *tableSchema)
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
                QList <QVariant> list;
                for(int i = 0; i < tableSchema->getFieldsCount(); i++)
                {
                    list.append(query->value(i));
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
    QList<QVariant> list = requestOne(sql, schema);
    if(!list.empty())
    {
        return list[0];
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

TableSchema * Query::generateTableSchema(QStringList list, TableSchema *table1, TableSchema *table2)
{
    TableSchema * schem = new TableSchema();
    if(list.empty())
    {
        list.append(table1->getFields());
        list.append(table2->getFields());
    }
    schem->setFields(list);
    return schem;
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
}

Query::~Query()
{

}

