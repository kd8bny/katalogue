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

bool Database::insertItemEntry(Item item)
{
    bool isInsert = false;
    QSqlQuery query;

    query.prepare(QString("INSERT INTO %1 (%2, %3, %4, %5, %6, %7, %8) "
        "VALUES (:name, :make, :model, :year, :type, :archived, :parent)").arg(
            TABLE_ITEMS, NAME, MAKE, MODEL, YEAR, TYPE, ARCHIVED, KEY_ITEM_ID));

    query.bindValue(":name", item.getName());
    query.bindValue(":make", item.getMake());
    query.bindValue(":model", item.getModel());
    query.bindValue(":year", item.getYear());
    query.bindValue(":type", item.getType());
    query.bindValue(":archived", item.getArchived());
    if(item.getParent() != -1){
        query.bindValue(":parent", item.getParent());
    }

    if (query.exec())
    {
        isInsert = true;
    } else {
        qDebug() << "Error inserting record " << query.lastError();
    }

    return isInsert;
}

bool Database::updateItemEntry(Item item)
{
    bool isUpdate = false;
    QSqlQuery query;

    query.prepare(QString("UPDATE %1 SET %2=:name, %3=:make, %4=:model, "
        "%5=:year, %6=:type, %7=:archived, %8=:parent WHERE id=:id").arg(
            TABLE_ITEMS, NAME, MAKE, MODEL, YEAR, TYPE, ARCHIVED, KEY_ITEM_ID));

    query.bindValue(":name", item.getName());
    query.bindValue(":make", item.getMake());
    query.bindValue(":model", item.getModel());
    query.bindValue(":year", item.getYear());
    query.bindValue(":type", item.getType());
    query.bindValue(":archived", item.getArchived());

    if (item.getParent() != -1) {
        query.bindValue(":parent", item.getParent());
    }

    query.bindValue(":id", item.getId());

    if (query.exec()) {
        isUpdate = true;
    } else {
        qDebug() << "Error updating record " << query.lastError();
        qDebug() << query.lastQuery();
    }

    return isUpdate;
}

bool Database::deleteItemEntry(int id)
{
    bool isDelete = false;
    QSqlQuery query;

    query.prepare(QString("DELETE FROM %1 WHERE id=:itemId").arg(TABLE_ITEMS));
    query.bindValue(":itemId", id);

    if(query.exec()){
        isDelete = true;
    } else {
        qDebug() << "Error removing record " << query.lastError().text();
    }

    return isDelete;
}

bool Database::insertAttributeEntry(Attribute attribute)
{
    bool isInsert = false;
    QSqlQuery query;

    query.prepare(QString(
        "INSERT INTO %1 (%2, %3, %4, %5) VALUES (:key, :value, :category, :itemId)").arg(
            TABLE_ATTRIBUTES, KEY, VALUE, CATEGORY, KEY_ITEM_ID));

    query.bindValue(":key", attribute.getKey());
    query.bindValue(":value", attribute.getValue());
    query.bindValue(":category", attribute.getCategory());
    query.bindValue(":itemId", attribute.getItemId());

    if(query.exec()){
        isInsert = true;
    } else {
        qDebug() << "Error inserting record " << TABLE_ATTRIBUTES;
        qDebug() << query.lastError();
    }

    return isInsert;
}

bool Database::updateAttributeEntry(Attribute attribute)
{
    bool isUpdate = false;
    QSqlQuery query;

    query.prepare(QString(
        "UPDATE %1 SET %2=:key, %3=:value, %4=:category WHERE id=:attributeId").arg(
            TABLE_ATTRIBUTES, KEY, VALUE, CATEGORY));

    query.bindValue(":key", attribute.getKey());
    query.bindValue(":value", attribute.getValue());
    query.bindValue(":category", attribute.getCategory());
    query.bindValue(":itemId", attribute.getItemId());

    query.bindValue(":attributeId", attribute.getId());

    if(query.exec()){
        isUpdate = true;
    } else {
        qDebug() << "Error updating record " << TABLE_ATTRIBUTES;
        qDebug() << query.lastError();
    }

    return isUpdate;
}

bool Database::deleteAttributeEntry(int attributeId)
{
    bool isDelete = false;
    QSqlQuery query;

    query.prepare(
        QString("DELETE FROM %1 WHERE id=:attributeId").arg(TABLE_ATTRIBUTES));
    query.bindValue(":attributeId", attributeId);

    if(query.exec()){
        isDelete = true;
    } else {
        qDebug() << "Error removing record " << query.lastError().text();
    }

    return isDelete;
}

bool Database::insertEventEntry(Event event)
{
    bool isInsert = false;
    QSqlQuery query;

    query.prepare(QString(
        "INSERT INTO %1 ( %2, %3, %4, %5, %6, %7, %8) VALUES "
        "(:date, :event, :cost, :odometer, :category, :comment, :itemId)").arg(
            TABLE_EVENTS, DATE, EVENT, COST, ODOMETER, CATEGORY, COMMENT, KEY_ITEM_ID));

    query.bindValue(":date", event.getDate());
    query.bindValue(":event", event.getEvent());
    query.bindValue(":cost", event.getCost());
    query.bindValue(":odometer", event.getOdometer());
    query.bindValue(":category", event.getCategory());
    query.bindValue(":comment", event.getComment());
    query.bindValue(":itemId", event.getItemId());

    if(query.exec()){
        isInsert = true;
    } else {
        qDebug() << "Error inserting record " << TABLE_EVENTS;
        qDebug() << query.lastError();
        qDebug() << query.lastQuery();
    }

    return isInsert;
}

bool Database::updateEventEntry(Event event)
{
    bool isUpdate = false;
    QSqlQuery query;

    query.prepare(QString(
        "UPDATE %1 SET %2=:date, %3=:event, %4=:cost, %5=:odometer, %6=:category, %7=:comment WHERE id=:eventId").arg(
            TABLE_EVENTS, DATE, EVENT, COST, ODOMETER, CATEGORY, COMMENT));

    query.bindValue(":date", event.getDate());
    query.bindValue(":event", event.getEvent());
    query.bindValue(":cost", event.getCost());
    query.bindValue(":odometer", event.getOdometer());
    query.bindValue(":category", event.getCategory());
    query.bindValue(":comment", event.getComment());

    query.bindValue(":eventId", event.getId());

    if(query.exec()){
        isUpdate = true;
    } else {
        qDebug() << "Error updating record " << TABLE_EVENTS;
        qDebug() << query.lastError();
        qDebug() << query.lastQuery();
    }

    return isUpdate;
}

bool Database::deleteEventEntry(int id)
{
    bool isDelete = false;
    QSqlQuery query;

    query.prepare(
        QString("DELETE FROM %1 WHERE id=:eventId").arg(TABLE_EVENTS));
    query.bindValue(":eventId", id);

    if(query.exec()){
        isDelete = true;
    } else {
        qDebug() << "Error removing record " << query.lastError().text();
    }

    return isDelete;
}

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
        ODOMETER    " REAL, "
        CATEGORY    " TEXT, "
        COMMENT     " VARCHAR(255), "
        KEY_ITEM_ID " INT NOT NULL, "
        "CONSTRAINT itemId FOREIGN KEY (" KEY_ITEM_ID ") REFERENCES " TABLE_ITEMS "(id) "
        "ON DELETE CASCADE ON UPDATE CASCADE)";

    if(!query.exec(queryItems)){
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

    isSchemaCreate = true;

    return isSchemaCreate;
}
