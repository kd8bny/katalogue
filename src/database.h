#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>
#include <QUuid>

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


class Database : public QObject
{
    Q_OBJECT

public:
    explicit Database(QObject *parent = nullptr);
    ~Database();
    bool connect(QString path);
    Q_INVOKABLE bool insertItemEntry(QString name, QString make, QString model,
        QString year, QString category, QString parent);
    Q_INVOKABLE bool updateItemEntry(QString itemID, QString name, QString make,
        QString model, QString year, QString type, QString archived, QString parent);
    Q_INVOKABLE bool deleteItemEntry(QString item_id);

    Q_INVOKABLE bool insertAttributeEntry(QString item_id, QString label,
        QString key, QString value);
    Q_INVOKABLE bool updateAttributeEntry(QString attributeId, QString key,
        QString value, QString category);
    Q_INVOKABLE bool deleteAttributeEntry(QString attributeId);

    Q_INVOKABLE bool insertEventEntry(QString date, QString task, QString cost,
        QString type, QString category, QString comment, QString item_id);
    Q_INVOKABLE bool updateEventEntry(QString eventId, QString date, QString event, QString cost, 
    QString type, QString category, QString comment);

private:
    QSqlDatabase db;

private:
    bool initializeSchema();
    // void initializeDefaults();
    void initializeDemoEntry();
    // bool insertDefaultEntry(QString type, QString value);
};
