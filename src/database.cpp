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
            qDebug() << "New database, creating tables";
            this->initializeSchema();
        }
        isDBOpen = true;
    }

    return isDBOpen;
}

bool Database::insertItemEntry()
{
    QSqlQuery query;
    bool isInsert = false;

    // query.prepare("INSERT INTO " TABLE_ITEMS " ("
    //     TABLE_NAME ", " TABLE_CATEGORY ", " TABLE_YEAR ", "
    //     TABLE_NICK ", " TABLE_ARCHIVED") "
    //     "VALUES (:name, :category, :year, :nick, :archived)");

    // query.bindValue(":name", data[0].toString());
    // query.bindValue(":category", data[1].toString());
    // query.bindValue(":year", data[2].toInt());
    // query.bindValue(":nick",       data[1].toString());
    // query.bindValue(":archived",         data[2].toInt());
 
 
    if(query.exec()){
        isInsert = true;
    } else {
        qDebug() << "Error inserting record " << TABLE_ITEMS;
        qDebug() << query.lastError().text();
    }

    return isInsert;
}

bool Database::initializeSchema()
{
    QSqlQuery query;
    bool isSchemaCreate = false;

    const QString queryItems = "CREATE TABLE " TABLE_ITEMS
        " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        TABLE_UID       " TEXT NOT NULL, "
        TABLE_NAME      " TEXT NOT NULL, "
        TABLE_MAKE      " TEXT, "
        TABLE_MODEL     " TEXT, "
        TABLE_YEAR      " INT, "
        TABLE_GROUP     " TEXT, "
        TABLE_CATEGORY  " TEXT NOT NULL, "
        TABLE_ARCHIVED  " BOOLEAN NOT NULL CHECK (" TABLE_ARCHIVED " IN (0, 1)))";

    const QString queryMaintenance = "CREATE TABLE " TABLE_MAINTENANCE
        " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        TABLE_UID       " TEXT NOT NULL, "
        TABLE_DATE      " DATE  NOT NULL,"
        TABLE_TASK      " TEXT  NOT NULL,"
        TABLE_COST      " REAL,"
        TABLE_TYPE      " TEXT,"
        TABLE_COMMENT   " VARCHAR(255))";


    query.exec(queryItems);
    if(!query.lastError().NoError){ // TODO this doesnt quite work yet
        qDebug() << queryItems;
        qDebug() << query.lastError();
        isSchemaCreate = true;
    }

    query.exec(queryMaintenance);
    if(!query.lastError().NoError){
        qDebug() << queryMaintenance;
        qDebug() << query.lastError();
        isSchemaCreate = true;
    }

    return isSchemaCreate;
}