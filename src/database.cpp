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
    QString year, QString category, QString group)
{
    bool isInsert = false;
    QSqlQuery query;

    query.prepare(QString("INSERT INTO %1 (%2, %3, %4, %5, %6, %7, %8) "
        "VALUES (:name, :make, :model, :year, :category, :group, :archived)").arg(
            TABLE_ITEMS, TABLE_NAME,  TABLE_MAKE, TABLE_MODEL, TABLE_YEAR,
            TABLE_CATEGORY, TABLE_GROUP, TABLE_ARCHIVED));

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
        qDebug() << "Error inserting record " << query.lastError();
    }

    return isInsert;
}

// bool Database::updateItemEntry(QString name, QString make, QString model,
//     QString year, QString category, QString group)
// {
    bool isInsert = false;
    // QSqlQuery query;

    // query.prepare(QString("UPDATE %1 SET %2=
    // .arg(TABLE_ITEMS, 
    //     "INSERT INTO "  " ("
    //     TABLE_NAME ", " TABLE_MAKE ", " TABLE_MODEL ", " TABLE_YEAR ", "
    //     TABLE_CATEGORY ", " TABLE_GROUP ", " TABLE_ARCHIVED")"
    //     " VALUES (:name, :make, :model, :year, :category, "
    //     ":view, :archived)");

    // query.bindValue(":name", name);
    // query.bindValue(":make", make);
    // query.bindValue(":model", model);
    // query.bindValue(":year", year.toInt());
    // query.bindValue(":category", category);
    // query.bindValue(":group", group); 
    // query.bindValue(":archived", 0);

    // if(query.exec()){
    //     isInsert = true;
    // } else {
    //     qDebug() << "Error inserting record " << query.lastError().text();
    // }

//     return isInsert;
// }

bool Database::deleteItemEntry(QString item_id)
{
    bool isDelete = false;
    QSqlQuery query;

    query.prepare(QString("DELETE FROM %1 WHERE id=:item_id").arg(TABLE_ITEMS));
    query.bindValue(":item_id", item_id.toInt());

    if(query.exec()){
        isDelete = true;
    } else {
        qDebug() << "Error removing record " << query.lastError().text();
    }

    return isDelete;
}

bool Database::insertAttributeEntry(QString item_id, QString label, QString key, QString value)
{
    bool isInsert = false;
    QSqlQuery query;

    query.prepare(QString(
        "INSERT INTO %1 (%2, %3, %4, %5) VALUES (:label, :key, :value, :item_id)").arg(
            TABLE_ATTRIBUTES, TABLE_LABEL, TABLE_KEY, TABLE_VALUE, TABLE_ITEM_ID));

    query.bindValue(":label", label);
    query.bindValue(":key", key);
    query.bindValue(":value", value);
    query.bindValue(":item_id", item_id);

    if(query.exec()){
        isInsert = true;
    } else {
        qDebug() << "Error inserting record " << TABLE_ATTRIBUTES;
        qDebug() << query.lastError();//.text();
        qDebug() << query.lastQuery();
        qDebug() << label << key << value << item_id;
    }

    return isInsert;
}

bool Database::insertEventEntry(QString date, QString task, QString cost, 
    QString type, QString category, QString comment, QString item_id)
{
    bool isInsert = false;
    QSqlQuery query;

    query.prepare("INSERT INTO " TABLE_EVENTS " ("
        TABLE_DATE ", " TABLE_TASK ", " TABLE_COST ", "
        TABLE_TYPE ", " TABLE_CATEGORY  ", " TABLE_COMMENT ", " TABLE_ITEM_ID")"
        " VALUES (:date, :task, :cost, :type, :category, :comment, :item_id)");

    query.bindValue(":date", date);
    query.bindValue(":task", task);
    query.bindValue(":cost", cost);
    query.bindValue(":type", type);
    query.bindValue(":category", category);
    query.bindValue(":comment", comment);
    query.bindValue(":item_id", item_id);

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
        TABLE_NAME      " TEXT NOT NULL, "
        TABLE_MAKE      " TEXT, "
        TABLE_MODEL     " TEXT, "
        TABLE_YEAR      " INT, "
        TABLE_GROUP     " TEXT, "
        TABLE_CATEGORY  " TEXT, "
        TABLE_ARCHIVED  " BOOLEAN NOT NULL CHECK (" TABLE_ARCHIVED " IN (0, 1)))";

    const QString queryAttributes = "CREATE TABLE " TABLE_ATTRIBUTES
        " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        TABLE_KEY       " TEXT NOT NULL, "
        TABLE_VALUE     " TEXT NOT NULL, "
        TABLE_LABEL     " TEXT, "
        TABLE_ITEM_ID   " INT NOT NULL, "
        "CONSTRAINT item_id FOREIGN KEY (" TABLE_ITEM_ID ") REFERENCES " TABLE_ITEMS "(id) "
        "ON DELETE CASCADE ON UPDATE CASCADE)";

    const QString queryEvent = "CREATE TABLE " TABLE_EVENTS
        " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        TABLE_DATE      " DATE NOT NULL, "
        TABLE_TASK      " TEXT NOT NULL, "
        TABLE_COST      " REAL, "
        TABLE_TYPE      " TEXT, "
        TABLE_CATEGORY  " TEXT, "
        TABLE_COMMENT   " VARCHAR(255), "
        TABLE_ITEM_ID   " INT NOT NULL, "
        "CONSTRAINT item_id FOREIGN KEY (" TABLE_ITEM_ID ") REFERENCES " TABLE_ITEMS "(id) "
        "ON DELETE CASCADE ON UPDATE CASCADE)";

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
        qDebug() << queryAttributes;
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

    this->insertAttributeEntry("1", "Default", "Engine", "4.6L V8");
    this->insertEventEntry("2022-05-22", "Check Oil", "0.00", "checkup",
        "auto", "Comment about event", "1");
}
