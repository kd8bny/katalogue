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
#define TABLE_UUID "UUID"

#define TABLE_ITEMS "ITEMS"
#define TABLE_NAME "NAME"
#define TABLE_MAKE "MAKE"
#define TABLE_MODEL "MODEL"
#define TABLE_YEAR "YEAR"
#define TABLE_CATEGORY "CATEGORY"
#define TABLE_GROUP "USERGROUP"
#define TABLE_ARCHIVED "ARCHIVED"

#define TABLE_EVENTS "EVENTS"
#define TABLE_DATE "DATE"
#define TABLE_TASK "TASK"
#define TABLE_COST "COST"
#define TABLE_TYPE "TYPE"
#define TABLE_COMMENT "COMMENT"

#define TABLE_ATTRIBUTES "ATTRIBUTES"
#define TABLE_LABEL "LABEL"
#define TABLE_KEY "KEY"
#define TABLE_VALUE "VALUE"


class Database : public QObject
{
    Q_OBJECT

public:
    explicit Database(QObject *parent = nullptr);
    ~Database();
    bool connect();
    Q_INVOKABLE bool insertItemEntry(QString name, QString make, QString model,
        QString year, QString category, QString group);
    Q_INVOKABLE bool insertAttributeEntry(QString uuid, QString label,
        QString key, QString value);
    Q_INVOKABLE bool insertEventEntry(QString uuid, QString date, QString task,
        QString cost, QString type, QString category, QString comment);

private:
    QSqlDatabase db;

private:
    bool initializeSchema();
    void initializeDemoEntry();
};
