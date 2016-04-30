#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QtSql>
#include "tableschema.h"

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = 0);
    Model(Model const & obj, QObject *parent = 0);
    ~Model();

    Model &                     operator = (Model const & obj);
    bool                        operator == (const Model & right);
    bool                        operator != (const Model & right);

    void                        setField(QString fieldName, QVariant value);
    void                        setSchema(TableSchema * tableSschema);
    void                        setRelation(Model *outModel, QString relationName);
    void                        setIsNew(bool value);

    TableSchema *               getSchema();
    QVariant                    getRecord(QString fieldName);
    QStringList                 getFields();
    bool                        getIsNew();

    bool                        save(QStringList fields = {});
    bool                        remove();

    void                        print();    //--

protected:
    QMap <QString, QVariant>    record;     //запись
    bool                        isNew;      //флаг редактирования записи в БД
    QSqlDatabase                database;   //объект по работе с БД
    TableSchema                 *schema;
    relation_s                  relation;

    virtual void                copy(const Model & obj);
    void                        connectToDB();
    bool                        insert();
    bool                        update(QStringList fields = {});
    bool                        execQuery(QString sql);
    QString                     getString(QVariant value);

signals:

public slots:
};

#endif // MODEL_H
