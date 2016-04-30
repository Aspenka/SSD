#ifndef QUERY_H
#define QUERY_H

#include <QObject>
#include <QList>
#include <QString>
#include <QtSql>
#include "tableschema.h"
#include "model.h"

typedef QList<QList <QVariant>> matrix_t;

class Query : public QObject
{
    Q_OBJECT
public:
    explicit Query(TableSchema *tableSchema, QObject *parent = 0);
    Query(Query const & obj, QObject *parent = 0);
    ~Query();

    void                setWhere(QString fieldName, QVariant value );
    void                setOrder(QString fieldName );
    void                setSelectedFields( QStringList fields );
    void                setLimit( QString str );
    void                withOuter(QString relationName, QString inField, QString outField);
    void                withInner(QString relationName, QString inField, QString outField);
    void                clear();

    Model *             getOne();
    Model *             getEmpty();
    QList <Model *>     getAll();
    int                 count(QString fieldName = "*");
    QVariant            getlastID();

    Query &             operator = (Query const & obj);
    bool                operator == (const Query & right);
    bool                operator != (const Query & right);

private:
    QString             where,
                        order,
                        selectedFields,
                        limit,
                        countString,
                        outerJoin,
                        innerJoin,
                        sqlString;
    TableSchema         *schema;
    QSqlDatabase        database;   //объект по работе с БД
    QStringList         selectedList;

    void                copy(Query const & obj);
    void                connectToDB();

    QString             generateSQL();
    matrix_t            requestMany(QString sql, TableSchema * tableSchema);
    QList<QVariant>     requestOne(QString sql, TableSchema * tableSchema);
    Model *             appendRecords(TableSchema *tableSchema, QList<QVariant> values, Model *& model);
    QString             listToString(QStringList list);
    TableSchema *       generateTableSchema(QStringList list, TableSchema * table1, TableSchema * table2);  //--
signals:

public slots:
};

#endif // QUERY_H
