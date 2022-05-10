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
#define TABLE_UID "UID"

#define TABLE_ITEMS "ITEMS"
#define TABLE_NAME "NAME"
#define TABLE_MAKE "MAKE"
#define TABLE_MODEL "MODEL"
#define TABLE_YEAR "YEAR"
#define TABLE_CATEGORY "CATEGORY"
#define TABLE_GROUP "USERGROUP"
#define TABLE_ARCHIVED "ARCHIVED"

#define TABLE_MAINTENANCE "MAINTENANCE"
#define TABLE_TASK "TASK"
#define TABLE_DATE "DATE"
#define TABLE_COST "COST"
#define TABLE_TYPE "TYPE"
#define TABLE_COMMENT "COMMENT"

#define TABLE_ATTRIBUTES "ATTRIBUTES"
#define TABLE_KEY "KEY"
#define TABLE_VALUE "VALUE"

typedef struct {
    char name[15];
    char make[15];
    char model[15];
    int year;
    char category[15];
    char group[15];
} Item;

class Database : public QObject
{
    Q_OBJECT

public:
    explicit Database(QObject *parent = nullptr);
    ~Database();
    bool connect();
    bool insertItemEntry(Item item);

private:
    QSqlDatabase db;

private:
    bool initializeSchema();
    void initializeDemoEntry();
};
