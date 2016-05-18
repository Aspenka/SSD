#ifndef QUERY_H
#define QUERY_H

#include <QObject>
#include <QList>
#include <QPair>
#include <QVariant>
#include <QString>
#include <QtSql>
#include "tableschema.h"
#include "model.h"

typedef QList <QList <QPair <QString, QVariant>>>   matrix_t;
typedef QList <QPair <QString, QVariant>>           list_t;

class Query : public QObject
{
    Q_OBJECT
public:
    explicit Query(TableSchema *tableSchema, QObject *parent = 0);
    Query(Query const & obj, QObject *parent = 0);
    ~Query();

    void                setWhere(QString fieldName, QVariant value );
    void                setOrder(QString fieldName );
    void                setSelectedFields(QStringList fields );
    void                setLimit( QString str );
    void                withOuter(QString relationName);
    void                withInner(QString relationName);

    void                updateSchema(Model* model);

    Model *             getOne();
    Model *             getEmpty();
    QList <Model *>     getAll();
    int                 count(QString fieldName = "*");
    QVariant            getlastID();

    Query &             operator = (Query const & obj);
    bool                operator == (const Query & right);
    bool                operator != (const Query & right);

    void                clear();

private:
    Model *             queryModel;
    QString             where,
                        order,
                        selectedFields,
                        limit,
                        countString,
                        outerJoin,
                        innerJoin,
                        sqlString,
                        rName;
    TableSchema *       schema;
    QSqlDatabase        database;   //объект по работе с БД

    QStringList         selectedList;

    void                copy(Query const & obj);
    void                connectToDB();

    QString             generateSQL();
    matrix_t            requestMany(QString sql);
    list_t              requestOne(QString sql);
    Model *             appendRecords(list_t values, Model *& model);
    relation_data_t     getRelationModel(list_t values, Model * & model);
    QString             listToString(QStringList list);
signals:

public slots:
};

#endif // QUERY_H
