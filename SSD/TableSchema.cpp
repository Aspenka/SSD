#include "tableschema.h"
#include <QDebug>

TableSchema::TableSchema(QObject *parent) : QObject(parent)
{

}

TableSchema::TableSchema(const TableSchema &obj, QObject *parent) : QObject(parent)
{
    copy(obj);
}

TableSchema *TableSchema::operator =(const TableSchema *obj)
{
    copy(*obj);
    return this;
}

bool relation_s::operator ==( const relation_s & right ) const
{
    if(( link == right.link ) &&
       ( type == right.type ) &&
       ( model == right.model ))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool relation_s::operator !=( const relation_s & right ) const
{
    return !( this == & right );
}

relation_s & relation_s::operator = (relation_s const & obj)
{
    link = obj.link;
    type = obj.type;
    model = obj.model;
}

bool TableSchema::operator == ( const TableSchema &right )
{
    if ( ( fields == right.fields ) &&
         ( primaryKey == right.primaryKey ) &&
         ( foreignKey == right.foreignKey ) &&
         ( tableName == right.tableName ) &&
         ( relations == right.relations ) &&
         ( relationData == right.relationData))
         //( multiRelationData == right.multiRelationData ))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool TableSchema::operator == ( const TableSchema *right )
{
    if ( ( fields == right->fields ) &&
         ( primaryKey == right->primaryKey ) &&
         ( foreignKey == right->foreignKey ) &&
         ( tableName == right->tableName ) &&
         ( relations == right->relations ) &&
         //( multiRelationData == right->multiRelationData ) &&
         ( relationData == right->relationData))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool TableSchema::operator != ( const TableSchema & right )
{
    return !( this == & right );
}

bool TableSchema::operator != ( const TableSchema * right )
{
    return !( *this == * right );
}

void TableSchema::setRelations(QString name, relation_s relation)
{
    relations.insert(name, relation);
}

void TableSchema::copy(const TableSchema &obj)
{
    fields = obj.fields;
    primaryKey = obj.primaryKey;
    foreignKey = obj.foreignKey;
    tableName = obj.tableName;
    relationData = obj.relationData;
    relations = obj.relations;
    //multiRelationData = obj.multiRelationData;
}

void TableSchema::setFields(QStringList fieldsList)
{
    fields = fieldsList;
}

void TableSchema::setPrimaryKey(QStringList pkList)
{
    primaryKey = pkList;
}

void TableSchema::setForeignKey(QStringList fkList)
{
    foreignKey = fkList;
}

void TableSchema::setTableName(QString tName)
{
    tableName = tName;
}

void TableSchema::setRelationData(relation_data_t data)
{
    relationData.unite(data);
}

/*void TableSchema::setRelationData(multi_relation_data_t data)
{
    multiRelationData = data;
}*/

QStringList TableSchema::getFields()
{
    return fields;
}

QStringList TableSchema::getPrimaryKey()
{
    return primaryKey;
}

QStringList TableSchema::getForeignKey()
{
    return foreignKey;
}

QString TableSchema::getTableName()
{
    return tableName;
}

int TableSchema::getFieldsCount()
{
    return fields.size();
}

relation_data_t TableSchema::getRelationData()
{
    return relationData;
}

relation_s TableSchema::getRelation(QString key)
{
    relation_s result;
    try
    {
        if(relations.contains(key))
        {
            result = relations.value(key);
        }
        else
        {
            throw key;
        }
    }
    catch (QString)
    {
        qDebug() << "[TableSchema::getRelation]:\tError! Relation " << key << " not found!";
    }
    return result;
}

bool TableSchema::checkField(QString fieldName)
{
    if(fields.contains(fieldName))
    {
        return true;
    }
    else
    {
        return false;
    }
}

TableSchema::~TableSchema()
{

}

