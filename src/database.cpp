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
        TABLE_UUID ", " TABLE_NAME ", " TABLE_MAKE ", " TABLE_MODEL ", "
        TABLE_YEAR ", " TABLE_CATEGORY ", " TABLE_GROUP ", " TABLE_ARCHIVED")"
        " VALUES (:uuid, :name, :make, :model, :year, :category, "
        ":view, :archived)");

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

bool Database::insertAttributeEntry(QString uuid, QString label, QString key, QString value)
{
    bool isInsert = false;
    QSqlQuery query;

    query.prepare("INSERT INTO " TABLE_ATTRIBUTES " ("
        TABLE_UUID ", " TABLE_LABEL ", " TABLE_KEY ", " TABLE_VALUE ")"
        " VALUES (:uuid, :label, :key, :value)");

    query.bindValue(":uuid", uuid);
    query.bindValue(":label", label);
    query.bindValue(":key", key); 
    query.bindValue(":value", value);

    if(query.exec()){
        isInsert = true;
    } else {
        qDebug() << "Error inserting record " << TABLE_ATTRIBUTES;
        qDebug() << query.lastError().text();
    }

    return isInsert;
}

bool Database::insertEventEntry(QString uuid, QString date, QString task,
                                QString cost, QString type, QString category, QString comment)
{
    bool isInsert = false;
    QSqlQuery query;

    query.prepare("INSERT INTO " TABLE_EVENTS " ("
        TABLE_UUID ", " TABLE_DATE ", " TABLE_TASK ", " TABLE_COST ", "
        TABLE_TYPE ", " TABLE_CATEGORY  ", " TABLE_COMMENT")"
        " VALUES (:uuid, :date, :task, :cost, :type, :category, :comment)");

    query.bindValue(":uuid", uuid);
    query.bindValue(":date", date);
    query.bindValue(":task", task);
    query.bindValue(":cost", cost);
    query.bindValue(":type", type);
    query.bindValue(":category", category);
    query.bindValue(":comment", comment); 

    if(query.exec()){
        isInsert = true;
    } else {
        qDebug() << "Error inserting record " << TABLE_EVENTS;
        qDebug() << query.lastError().text();
    }

    return isInsert;
}

bool Database::initializeSchema()
{
    bool isSchemaCreate = false;
    QSqlQuery query;


    const QString queryItems = "CREATE TABLE " TABLE_ITEMS
        " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        TABLE_UUID      " TEXT NOT NULL, "
        TABLE_NAME      " TEXT NOT NULL, "
        TABLE_MAKE      " TEXT, "
        TABLE_MODEL     " TEXT, "
        TABLE_YEAR      " INT, "
        TABLE_GROUP     " TEXT, "
        TABLE_CATEGORY  " TEXT, "
        TABLE_ARCHIVED  " BOOLEAN NOT NULL CHECK (" TABLE_ARCHIVED " IN (0, 1)))";

    const QString queryAttributes = "CREATE TABLE " TABLE_ATTRIBUTES
        " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        TABLE_UUID      " TEXT NOT NULL, "
        TABLE_LABEL     " TEXT NOT NULL, "
        TABLE_KEY       " TEXT NOT NULL, "
        TABLE_VALUE     " TEXT NOT NULL)";

    const QString queryEvent = "CREATE TABLE " TABLE_EVENTS
        " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        TABLE_UUID      " TEXT NOT NULL, "
        TABLE_DATE      " DATE NOT NULL, "
        TABLE_TASK      " TEXT NOT NULL, "
        TABLE_COST      " REAL, "
        TABLE_TYPE      " TEXT, "
        TABLE_CATEGORY  " TEXT, "
        TABLE_COMMENT   " VARCHAR(255))";

    bool isItems = false;
    bool isAttributes = false;
    bool isEvents = false;

    if(query.exec(queryItems)){
        isItems = true;
    }else{
        qDebug() << query.lastError();
    }

    if(query.exec(queryAttributes)){
        isAttributes = true;
    }else{
        qDebug() << query.lastError();
    }

    if(query.exec(queryEvent)){
        isEvents = true;
    }else{
        qDebug() << query.lastError();
    }

    if(isItems && isAttributes && isEvents){
        isSchemaCreate = true;
        this->initializeDemoEntry();
    }

    return isSchemaCreate;
}

void Database::initializeDemoEntry()
{
    this->insertItemEntry(
        "My Vehicle", "Ford", "Mustang", "2000", "auto", "My Group");

    QSqlQuery query;
    query.prepare("SELECT " TABLE_UUID " FROM " TABLE_ITEMS " WHERE id=1");
    if(!query.exec()){
        qDebug() << query.lastError();
    }
    query.next();
    QString uuid = query.value(0).toString();

    this->insertAttributeEntry(uuid, "Default", "Engine", "4.6L V8");
    this->insertEventEntry(uuid, "2022-05-22", "Check Oil", "0.00", "checkup",
        "auto", "Comment about event");
}
