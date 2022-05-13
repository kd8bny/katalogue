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

bool Database::insertItemEntry(QString name, QString make, QString model,
QString year, QString category, QString group)
{
    bool isInsert = false;
    QSqlQuery query;
    QString uuid = QUuid().createUuid().toString();

    query.prepare("INSERT INTO " TABLE_ITEMS " ("
        TABLE_UID ", " TABLE_NAME ", " TABLE_MAKE ", " TABLE_MODEL ", "
        TABLE_YEAR ", " TABLE_GROUP ", " TABLE_CATEGORY  ", " TABLE_ARCHIVED")"
        " VALUES (:uuid, :name, :make, :model, :year, :group, "
        ":category, :archived)");

    query.bindValue(":uuid", uuid);
    query.bindValue(":name", name);
    query.bindValue(":make", make);
    query.bindValue(":model", model);
    query.bindValue(":year", year.toInt());
    query.bindValue(":category", category);
    query.bindValue(":group", group); 
    query.bindValue(":archived", 0);

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
        TABLE_DATE      " DATE NOT NULL,"
        TABLE_TASK      " TEXT NOT NULL,"
        TABLE_COST      " REAL,"
        TABLE_TYPE      " TEXT,"
        TABLE_CATEGORY  " TEXT NOT NULL, "
        TABLE_COMMENT   " VARCHAR(255))";

    const QString queryAttributes = "CREATE TABLE " TABLE_ATTRIBUTES
        " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        TABLE_CATEGORY  " TEXT NOT NULL, "
        TABLE_KEY       " TEXT NOT NULL, "
        TABLE_VALUE     " TEXT NOT NULL)";


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

    query.exec(queryAttributes);
    if(!query.lastError().NoError){
        qDebug() << queryAttributes;
        qDebug() << query.lastError();
        isSchemaCreate = true;
    }

    this->initializeDemoEntry();

    return isSchemaCreate;
}

void Database::initializeDemoEntry()
{
    QStringList demoEntry = {"name", "make", "model", "1992", "category", "group"};

    // this->insertItemEntry(demoEntry);

}
