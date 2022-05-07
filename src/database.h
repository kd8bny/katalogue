#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

#define DATABASE_NAME "katalogue.db"
#define DATABASE_TYPE "QSQLITE"

#define TABLE_ITEMS "ITEMS"
#define TABLE_NAME "NAME"
#define TABLE_NICK "NICK"
#define TABLE_YEAR "YEAR"
#define TABLE_CATEGORY "CATEGORY"
#define TABLE_ARCHIVED "ARCHIVED"

#define TABLE_MAINTENANCE "MAINTENANCE"
#define TABLE_DATE "DATE"
#define TABLE_TASK "TASK"
#define TABLE_COST "COST"
#define TABLE_TYPE "TYPE"
#define TABLE_COMMENT "COMMENT"

class Database : public QObject
{
    Q_OBJECT

public:
    explicit Database(QObject *parent = nullptr);
    ~Database();
    bool connect();
    bool insertItemEntry();

private:
    QSqlDatabase db;

private:
    bool initializeSchema();
    // bool restoreDataBase();
    // void closeDataBase();
};
