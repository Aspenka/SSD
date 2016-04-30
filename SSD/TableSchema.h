#ifndef TABLESCHEMA_H
#define TABLESCHEMA_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QMap>

class Model;

enum    relationType_e
{
    ONE_TO_ONE = 1,
    ONE_TO_MANY = 2,
    MANY_TO_ONE = 3
};

struct relation_s
{
    QMap <QString, QString> link;
    int type {0};
    Model * Model;
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


    QStringList     getFields();
    QStringList     getPrimaryKey();
    QStringList     getForeignKey();
    QString         getTableName();
    int             getFieldsCount();
    relation_s      getRelation(QString key);

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

    void            copy(TableSchema const & obj);
signals:

public slots:
};

#endif // TABLESCHEMA_H
