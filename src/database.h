#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QFile>
#include <QDate>
#include <QDebug>
#include <QUuid>

#include "data/event.h"
#include "data/item.h"

#ifndef DATABASE_H
#define DATABASE_H

#define DATABASE_TYPE "QSQLITE"
#define DATABASE_NAME "katalogue.db"
#define USER_VERSION 1

#define TABLE_ITEMS "items"
#define TABLE_EVENTS "events"
#define TABLE_ATTRIBUTES "attributes"
#define TABLE_DEFAULTS "defaults"

#define KEY_ITEM_ID "fk_item_id"

#define ARCHIVED "archived"
#define CATEGORY "category"
#define COMMENT "comment"
#define COST "cost"
#define DATE "date"
#define GROUP "item_group"
#define KEY "key"
#define LABEL "label"
#define EVENT "event"
#define MAKE "make"
#define MODEL "model"
#define NAME "name"
#define TYPE "type"
#define VALUE "value"
#define YEAR "year"
#define ODOMETER "odometer"


class Database : public QObject
{
    Q_OBJECT

public:
    explicit Database(QObject *parent = nullptr);
    ~Database();
    bool connect(QString path);

    bool insertItemEntry(Item item);
    bool updateItemEntry(Item item);
    bool deleteItemEntry(int id);

    Q_INVOKABLE bool insertAttributeEntry(QString itemId, QString label,
        QString key, QString value);
    Q_INVOKABLE bool updateAttributeEntry(QString attributeId, QString key,
        QString value, QString category);
    Q_INVOKABLE bool deleteAttributeEntry(QString attributeId);

    bool insertEventEntry(Event event);
    bool updateEventEntry(Event event);
    bool deleteEventEntry(int id);

private:
    QSqlDatabase db;

    bool initializeSchema();
};

#endif
