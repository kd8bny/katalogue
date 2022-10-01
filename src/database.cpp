#include "database.h"

Database::Database(QObject *parent)
    : QObject(parent)
{

}
Database::~Database()
{

}

bool Database::connect(QString path)
{
    bool isDBOpen = false;

    db = QSqlDatabase::addDatabase(DATABASE_TYPE);
    db.setDatabaseName(QString("%1/%2").arg(path, DATABASE_NAME));

    if(db.open()){
        QSqlQuery query;

        // Check if the database is empty
        query.exec("PRAGMA user_version");
        query.next();
        int db_version = query.value(0).toInt();
        qDebug() << "database version: " << db_version;

        if(db_version == 0){
            qDebug() << "New database, creating tables";

            query.exec(QString("PRAGMA user_version = %1").arg(USER_VERSION));
            this->initializeSchema();
        }else if(db_version > USER_VERSION){
            qDebug() << "Database is newer than application";
        }else{
            // Enable ON CASCADE
            query.exec("PRAGMA foreign_keys = ON;");
            isDBOpen = true;
        }
    }

    return isDBOpen;
}

bool Database::insertItemEntry(QString name, QString make, QString model,
    QString year, QString type, QString parent)
{
    bool isInsert = false;
    QSqlQuery query;

    query.prepare(QString("INSERT INTO %1 (%2, %3, %4, %5, %6, %7, %8) "
        "VALUES (:name, :make, :model, :year, :type, :archived, :parent)").arg(
            TABLE_ITEMS, NAME,  MAKE, MODEL, YEAR, TYPE, ARCHIVED, KEY_ITEM_ID));

    query.bindValue(":name", name);
    query.bindValue(":make", make);
    query.bindValue(":model", model);
    query.bindValue(":year", year.toInt());
    query.bindValue(":type", type);
    query.bindValue(":archived", 0);
    if(parent != "NULL"){
        query.bindValue(":parent", parent);
    }

    if(query.exec()){
        isInsert = true;
    } else {
        qDebug() << type;
        qDebug() << parent;
        qDebug() << "Error inserting record " << query.lastError();
    }

    return isInsert;
}

bool Database::updateItemEntry(QString itemID, QString name, QString make, QString model,
    QString year, QString type, QString archived, QString parent)
{
    bool isUpdate = false;
    QSqlQuery query;

    query.prepare(QString("UPDATE %1 SET %2=:name, %3=:make, %4=:model, "
        "%5=:year, %6=:type, %7=:archived, %8=:parent WHERE id=:id").arg(
            TABLE_ITEMS, NAME,  MAKE, MODEL, YEAR, TYPE, ARCHIVED, KEY_ITEM_ID));

    query.bindValue(":name", name);
    query.bindValue(":make", make);
    query.bindValue(":model", model);
    query.bindValue(":year", year.toInt());
    query.bindValue(":type", type);
    query.bindValue(":archived", archived);
    query.bindValue(":parent", parent);
    query.bindValue(":id", itemID);

    if(query.exec()){
        isUpdate = true;
    } else {
        qDebug() << "Error updating record " << query.lastError();
    }

    return isUpdate;
}

bool Database::deleteItemEntry(QString itemId)
{
    bool isDelete = false;
    QSqlQuery query;

    query.prepare(QString("DELETE FROM %1 WHERE id=:itemId").arg(TABLE_ITEMS));
    query.bindValue(":itemId", itemId.toInt());

    if(query.exec()){
        isDelete = true;
    } else {
        qDebug() << "Error removing record " << query.lastError().text();
    }

    return isDelete;
}

bool Database::insertAttributeEntry(QString itemId, QString key, QString value, QString category)
{
    bool isInsert = false;
    QSqlQuery query;

    query.prepare(QString(
        "INSERT INTO %1 (%2, %3, %4, %5) VALUES (:key, :value, :category, :itemId)").arg(
            TABLE_ATTRIBUTES, KEY, VALUE, CATEGORY, KEY_ITEM_ID));

    query.bindValue(":key", key);
    query.bindValue(":value", value);
    query.bindValue(":category", category);
    query.bindValue(":itemId", itemId);

    if(query.exec()){
        isInsert = true;
    } else {
        qDebug() << "Error inserting record " << TABLE_ATTRIBUTES;
        qDebug() << query.lastError();
    }

    return isInsert;
}

bool Database::updateAttributeEntry(QString attributeId, QString key, QString value, QString category)
{
    bool isUpdate = false;
    QSqlQuery query;

    query.prepare(QString(
        "UPDATE %1 SET %2=:key, %3=:value, %4=:category WHERE id=:attributeId").arg(
            TABLE_ATTRIBUTES, KEY, VALUE, CATEGORY));

    query.bindValue(":key", key);
    query.bindValue(":value", value);
    query.bindValue(":category", category);
    query.bindValue(":attributeId", attributeId);

    if(query.exec()){
        isUpdate = true;
    } else {
        qDebug() << "Error updating record " << TABLE_ATTRIBUTES;
        qDebug() << query.lastError();
    }

    return isUpdate;
}

bool Database::insertEventEntry(QString itemId, QString date, QString event, QString cost, 
    QString type, QString category, QString comment)
{
    bool isInsert = false;
    QSqlQuery query;

    query.prepare(QString(
        "INSERT INTO %1 ( %2, %3, %4, %5, %6, %7, %8) VALUES "
        "(:date, :event, :cost, :category, :type, :comment, :itemId)").arg(
            TABLE_EVENTS, DATE, EVENT, COST, CATEGORY, TYPE, COMMENT, KEY_ITEM_ID));

    query.bindValue(":date", date);
    query.bindValue(":event", event);
    query.bindValue(":cost", cost);
    query.bindValue(":category", category);
    query.bindValue(":type", type);
    query.bindValue(":comment", comment);
    query.bindValue(":itemId", itemId);

    if(query.exec()){
        isInsert = true;
    } else {
        qDebug() << "Error inserting record " << TABLE_EVENTS;
        qDebug() << query.lastError();
    }

    return isInsert;
}

bool Database::updateEventEntry(QString eventId, QString date, QString event, QString cost, 
    QString type, QString category, QString comment)
{
    bool isUpdate = false;
    QSqlQuery query;

    query.prepare(QString(
        "UPDATE %1 SET %2=:date, %3=:event, %4=:cost, %5:category, %6:type, %7:comment, "
        "%8:eventId WHERE id=:eventId").arg(
            TABLE_EVENTS, DATE, EVENT, COST, CATEGORY, TYPE, COMMENT, KEY_ITEM_ID));

    query.bindValue(":date", date);
    query.bindValue(":event", event);
    query.bindValue(":cost", cost);
    query.bindValue(":category", category);
    query.bindValue(":type", type);
    query.bindValue(":comment", comment);
    query.bindValue(":eventId", eventId);

    if(query.exec()){
        isUpdate = true;
    } else {
        qDebug() << "Error updating record " << TABLE_EVENTS;
        qDebug() << query.lastError();
    }

    return isUpdate;
}

// bool Database::insertDefaultEntry(QString type, QString value)
// {
//     bool isInsert = false;
//     QSqlQuery query;

//     query.prepare(QString(
//         "INSERT INTO %1 (%2, %3, %4, %5) VALUES (:type, :key, :value, :itemId)").arg(
//             TABLE_DEFAULTS, TYPE, VALUE));

//     query.bindValue(":type", type);
//     query.bindValue(":value", value);

//     if(query.exec()){
//         isInsert = true;
//     } else {
//         qDebug() << "Error inserting record " << TABLE_ATTRIBUTES;
//         qDebug() << query.lastError();
//         qDebug() << query.lastQuery();
//     }

//     return isInsert;
// }

bool Database::initializeSchema()
{
    bool isSchemaCreate = false;
    QSqlQuery query;

    const QString queryItems = "CREATE TABLE " TABLE_ITEMS
        " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        NAME        " TEXT NOT NULL, "
        MAKE        " TEXT, "
        MODEL       " TEXT, "
        YEAR        " INT, "
        TYPE        " TEXT NOT NULL, "
        ARCHIVED    " BOOLEAN NOT NULL CHECK (" ARCHIVED " IN (0, 1)), "
        KEY_ITEM_ID " INT, "
        "FOREIGN KEY (" KEY_ITEM_ID ") REFERENCES " TABLE_ITEMS "(id) "
        "ON DELETE CASCADE ON UPDATE CASCADE)";

    const QString queryAttributes = "CREATE TABLE " TABLE_ATTRIBUTES
        " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        KEY           " TEXT NOT NULL, "
        VALUE         " TEXT NOT NULL, "
        CATEGORY      " TEXT, "
        KEY_ITEM_ID   " INT NOT NULL, "
        "CONSTRAINT itemId FOREIGN KEY (" KEY_ITEM_ID ") REFERENCES " TABLE_ITEMS "(id) "
        "ON DELETE CASCADE ON UPDATE CASCADE)";

    const QString queryEvents = "CREATE TABLE " TABLE_EVENTS
        " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        DATE        " DATE NOT NULL, "
        EVENT       " TEXT NOT NULL, "
        COST        " REAL, "
        CATEGORY    " TEXT, "
        TYPE        " TEXT, "
        COMMENT     " VARCHAR(255), "
        KEY_ITEM_ID " INT NOT NULL, "
        "CONSTRAINT itemId FOREIGN KEY (" KEY_ITEM_ID ") REFERENCES " TABLE_ITEMS "(id) "
        "ON DELETE CASCADE ON UPDATE CASCADE)";

    // const QString queryDefaults = "CREATE TABLE " TABLE_DEFAULTS
    //     " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
    //     TYPE    " TEXT NOT NULL, "
    //     VALUE   " TEXT NOT NULL)";

    if(!query.exec(queryItems)){
        qDebug() << queryItems;
        qDebug() << query.lastError();
        return false;
    }

    if(!query.exec(queryAttributes)){
        qDebug() << query.lastError();
        return false;
    }

    if(!query.exec(queryEvents)){
        qDebug() << query.lastError();
        return false;
    }

    // if(!query.exec(queryDefaults)){
    //     qDebug() << query.lastError();
    //     return false;
    // }

    // this->initializeDefaults();
    this->initializeDemoEntry();
    isSchemaCreate = true;

    return isSchemaCreate;
}

// void Database::initializeDefaults()
// {
//     const QStringList groups = {"Default"};
//     const QStringList types = {"Auto", "Yard", "Utility", "Home"};


//     for (const auto& group : groups)
//     {
//         this->insertDefaultEntry(GROUP, group);
//     }

//     for (const auto& type : types)
//     {
//         this->insertDefaultEntry(TYPE, type);
//     }
// }

void Database::initializeDemoEntry()
{
    this->insertItemEntry(
        "My Vehicle", "Ford", "Mustang", "2000", "Auto", "NULL");

    this->insertAttributeEntry("1", "Engine", "4.6L V8", "Engine Specs");
    this->insertEventEntry("1", "2022-05-22", "Idle Pulley", "100.00", "Auto",
        "maintenance", "I fixed this thing");
    this->insertEventEntry("1", "2022-06-22", "Check Oil", "0.00", "Auto",
        "log", "Seems to be 2qt low");
}
