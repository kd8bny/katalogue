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
    //db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName(DATABASE_NAME);

    // if(db.tables()){
    //db.tables()

    // }
    

    if(db.open()){
        isDBOpen = true;
    }
    this->initializeSchema();

    return isDBOpen;
}

bool Database::initializeSchema()
{
    QSqlQuery query;
    bool isSchemaCreate = false;

    const QString queryItems = "CREATE TABLE " TABLE_ITEMS
        " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        TABLE_NAME      " VARCHAR(255)    NOT NULL,"
        TABLE_YEAR      " VARCHAR(255)    NOT NULL,"
        TABLE_CATEGORY  " VARCHAR(255)    NOT NULL)";
        // TABLE_NICK    "VARCHAR(255)    NULLABLE)";

    // try{
    query.exec(queryItems);
    isSchemaCreate = true;
    qDebug() << queryItems;
    qDebug() << query.lastError();
    // }catch(QSqlError::TransactionError ex){
    //     //test
    // }

    return isSchemaCreate;
}