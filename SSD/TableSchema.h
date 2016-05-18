#ifndef TABLESCHEMA_H
#define TABLESCHEMA_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QMap>

class Model;

typedef QMap <QString, QList <Model *>> relation_data_t;
//typedef QMap <QString, QList <Model *>>   multi_relation_data_t;

enum    relationType_e
{
    ONE_TO_ONE = 1,
    ONE_TO_MANY = 2,
    MANY_TO_ONE = 3
};

struct relation_s
{
    QPair <QString,
           QString> link;
    int             type {0};
    Model *         model;

    bool            operator == (const relation_s & right) const;
    bool            operator != (const relation_s & right) const;
    relation_s &    operator = (relation_s const & obj);
};

class TableSchema : public QObject
{
    Q_OBJECT
public:
    explicit TableSchema(QObject *parent = 0);
    TableSchema(TableSchema const & obj, QObject *parent = 0);
    ~TableSchema();

    void            setFields(QStringList fieldsList);
    void            setPrimaryKey(QStringList pkList);
    void            setForeignKey(QStringList fkList);
    void            setTableName(QString tName);
    void            setRelations(QString name, relation_s relation);

    void            setRelationData(relation_data_t data);
    //void            setRelationData(multi_relation_data_t data);

    QStringList     getFields();

    QStringList     getPrimaryKey();
    QStringList     getForeignKey();
    QString         getTableName();
    int             getFieldsCount();
    relation_s      getRelation(QString key);
    relation_data_t getRelationData();

    bool            checkField(QString fieldName);

    TableSchema &   operator = (TableSchema const & obj);
    TableSchema *   operator = (TableSchema const * obj);
    bool            operator == (const TableSchema & right);
    bool            operator == (const TableSchema * right);
    bool            operator != (const TableSchema & right);
    bool            operator != (const TableSchema * right);

private:
    QStringList     fields,
                    primaryKey,
                    foreignKey;
    QString         tableName;
    int             fieldsCount;

    QMap <QString,
    relation_s>     relations;

    relation_data_t        relationData;

    //multi_relation_data_t  multiRelationData;

    void            copy(TableSchema const & obj);
signals:

public slots:
};

#endif // TABLESCHEMA_H
