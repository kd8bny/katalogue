#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>
#include <QUuid>

#define DATABASE_NAME "katalogue.db"
#define DATABASE_TYPE "QSQLITE"
#define USER_VERSION 1

#define TABLE_ITEM_ID "fk_item_id"

#define TABLE_ITEMS "items"
#define TABLE_NAME "name"
#define TABLE_MAKE "make"
#define TABLE_MODEL "model"
#define TABLE_YEAR "year"
#define TABLE_CATEGORY "category"
#define TABLE_GROUP "user_group"
#define TABLE_ARCHIVED "archived"

#define TABLE_EVENTS "events"
#define TABLE_DATE "date"
#define TABLE_TASK "task"
#define TABLE_COST "cost"
#define TABLE_TYPE "type"
#define TABLE_COMMENT "comment"

#define TABLE_ATTRIBUTES "attributes"
#define TABLE_LABEL "label"
#define TABLE_KEY "key"
#define TABLE_VALUE "value"


class Database : public QObject
{
    Q_OBJECT

public:
    explicit Database(QObject *parent = nullptr);
    ~Database();
    bool connect();
    Q_INVOKABLE bool insertItemEntry(QString name, QString make, QString model,
        QString year, QString category, QString group);
    Q_INVOKABLE bool insertAttributeEntry(QString item_id, QString label,
        QString key, QString value);
    Q_INVOKABLE bool insertEventEntry(QString date, QString task, QString cost,
        QString type, QString category, QString comment, QString item_id);
    Q_INVOKABLE bool deleteItemEntry(QString item_id);
    // Q_INVOKABLE bool updateItemEntry(QString name, QString make, QString model,
    //     QString year, QString category, QString group);

private:
    QSqlDatabase db;

private:
    bool initializeSchema();
    void initializeDemoEntry();
};
