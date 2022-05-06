#include "database.h"

Database::Database(QObject *parent)
    : QObject(parent)
{

}
Database::~Database()
{

}

bool Database::connect()
{
    bool isDBOpen = false;

    db = QSqlDatabase::addDatabase(DATABASE_TYPE);
    db.setDatabaseName(DATABASE_NAME);

    if(db.open()){
        // Check if the database is empty
        if(db.tables().size() == 0){
            this->initializeSchema();
        }
        isDBOpen = true;
    }

    return isDBOpen;
}

bool Database::initializeSchema()
{
    QSqlQuery query;
    bool isSchemaCreate = false;

    const QString queryItems = "CREATE TABLE " TABLE_ITEMS
        " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        TABLE_NAME      " TEXT  NOT NULL,"
        TABLE_CATEGORY  " TEXT  NOT NULL,"
        TABLE_YEAR      " INT,"
        TABLE_NICK      " TEXT)";

    query.exec(queryItems);

    const QString queryMaintenance = "CREATE TABLE " TABLE_MAINTENANCE
        " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        TABLE_DATE      " NUMERIC   NOT NULL," //TODO type?
        TABLE_TASK      " TEXT      NOT NULL,"
        TABLE_COST      " REAL,"
        TABLE_TYPE      " TEXT,"
        TABLE_COMMENT   " VARCHAR(255))";

    // try{
    query.exec(queryMaintenance);
    isSchemaCreate = true;
    qDebug() << queryItems;
    qDebug() << query.lastError();
    // }catch(QSqlError::TransactionError ex){
    //     //test
    // }

    return isSchemaCreate;
}