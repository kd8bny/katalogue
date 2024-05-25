#include "database.h"

const QString Database::TABLE_ATTRIBUTES = QStringLiteral("attributes");
const QString Database::TABLE_EVENTS = QStringLiteral("events");
const QString Database::TABLE_ITEMS = QStringLiteral("items");
const QString Database::TABLE_NOTES = QStringLiteral("notes");
const QString Database::TABLE_TASKS = QStringLiteral("tasks");
const QString Database::TABLE_DEFAULTS = QStringLiteral("defaults");

const QString Database::ARCHIVED = QStringLiteral("archived");
const QString Database::CATEGORY = QStringLiteral("category");
const QString Database::COMMENT = QStringLiteral("comment");
const QString Database::COST = QStringLiteral("cost");
const QString Database::CREATED = QStringLiteral("created");
const QString Database::DATE = QStringLiteral("date");
const QString Database::GROUP = QStringLiteral("item_group");
const QString Database::KEY = QStringLiteral("key");
const QString Database::LABEL = QStringLiteral("label");
const QString Database::EVENT = QStringLiteral("event");
const QString Database::MAKE = QStringLiteral("make");
const QString Database::MODEL = QStringLiteral("model");
const QString Database::MODIFIED = QStringLiteral("modified");
const QString Database::NAME = QStringLiteral("name");
const QString Database::USER_ORDER = QStringLiteral("user_order");
const QString Database::TYPE = QStringLiteral("type");
const QString Database::VALUE = QStringLiteral("value");
const QString Database::YEAR = QStringLiteral("year");
const QString Database::ODOMETER = QStringLiteral("odometer");
const QString Database::TITLE = QStringLiteral("title");
const QString Database::NOTE_CONTENT = QStringLiteral("note_content");
const QString Database::DESCRIPTION = QStringLiteral("description");
const QString Database::DUE_DATE = QStringLiteral("due_date");
const QString Database::STATUS = QStringLiteral("status");

const QString Database::KEY_ITEM_ID = QStringLiteral("fk_item_id");


Database::Database(QObject *parent)
    : QObject(parent)
{

}

Database::~Database()
{

}

const QString Database::getCurrentTime()
{
    QDateTime dt = QDateTime::currentDateTime();
    dt.setTimeSpec(Qt::UTC);

    QString date = QDateTime::currentDateTime().toString(Qt::ISODate);
    qDebug() << "getCurrentTime:" << date;

    return date;
}

bool Database::connect(QString path)
{
    db = QSqlDatabase::addDatabase(DATABASE_TYPE);
    db.setDatabaseName(QStringLiteral("%1/%2").arg(path, this->DATABASE_NAME));

    if(db.open()){
        QSqlQuery query;

        // Check if the database is empty
        query.exec(QStringLiteral("PRAGMA user_version"));
        query.next();
        int db_version = query.value(0).toInt();
        qDebug() << "database version: " << db_version;

        if(db_version == 0){
            qDebug() << "New database, creating tables";

            if(!this->initializeSchema()){
                qDebug() << "Failed to initialize database schema";
                return false;
            }

            query.exec(QStringLiteral("PRAGMA user_version = %1").arg(this->DATABASE_VERSION));
            query.exec(QStringLiteral("PRAGMA foreign_keys = ON;"));

        }else if(db_version > this->DATABASE_VERSION){
            qDebug() << "Database is newer than application";
            //TODO Implement upgrade when needed
        }

        // Enable ON CASCADE each time db is open with foreign_key
        query.exec(QStringLiteral("PRAGMA foreign_keys = ON;"));
    }

    return true;
}

bool Database::insertItemEntry(const Item &item)
{
    bool isInsert = false;
    QSqlQuery query;

    query.prepare(QStringLiteral("INSERT INTO %1 (%2, %3, %4, %5, %6, %7, %8, %9, %10, %11) "
        "VALUES (:created, :modified, :name, :make, :model, :year, :type, :archived, :user_order, :parent)").arg(
            TABLE_ITEMS, CREATED, MODIFIED, NAME, MAKE, MODEL, YEAR, TYPE, ARCHIVED, USER_ORDER, KEY_ITEM_ID));

    QString currentTime = this->getCurrentTime();

    query.bindValue(QStringLiteral(":created"), currentTime);
    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":name"), item.getName());
    query.bindValue(QStringLiteral(":make"), item.getMake());
    query.bindValue(QStringLiteral(":model"), item.getModel());
    query.bindValue(QStringLiteral(":year"), item.getYear());
    query.bindValue(QStringLiteral(":type"), item.getType());
    query.bindValue(QStringLiteral(":user_order"), item.getUserOrder());
    query.bindValue(QStringLiteral(":archived"), item.getArchived());
    if(item.getParent() != -1){
        qDebug() << "Item is a Component id: " << item.getParent();
        query.bindValue(QStringLiteral(":parent"), item.getParent());
    }

    if (query.exec())
    {
        isInsert = true;
    } else {
        qDebug() << "Error inserting record " << query.lastError();
    }

    return isInsert;
}

bool Database::updateItemEntry(const Item &item)
{
    bool isUpdate = false;
    QSqlQuery query;

    query.prepare(QStringLiteral("UPDATE %1 SET %2=:modified, %3=:name, %4=:make, %5=:model, "
        "%6=:year, %7=:type, %8=:archived, %9=:parent WHERE id=:id").arg(
            TABLE_ITEMS, MODIFIED, NAME, MAKE, MODEL, YEAR, TYPE, ARCHIVED, KEY_ITEM_ID));

    QString currentTime = this->getCurrentTime();

    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":name"), item.getName());
    query.bindValue(QStringLiteral(":make"), item.getMake());
    query.bindValue(QStringLiteral(":model"), item.getModel());
    query.bindValue(QStringLiteral(":year"), item.getYear());
    query.bindValue(QStringLiteral(":type"), item.getType());
    query.bindValue(QStringLiteral(":archived"), item.getArchived());

    if (item.getParent() != -1) {
        query.bindValue(QStringLiteral(":parent"), item.getParent());
    }

    query.bindValue(QStringLiteral(":id"), item.getId());

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

    query.prepare(
        QStringLiteral("DELETE FROM %1 WHERE id=:itemId").arg(TABLE_ITEMS));
    qDebug() << QStringLiteral("DELETE FROM %1 WHERE id=:itemId").arg(TABLE_ITEMS);

    query.bindValue(QStringLiteral(":itemId"), id);

    qDebug() << query.lastQuery();
    qDebug() << id;

    if(query.exec()){
        isDelete = true;
        qDebug() << query.lastQuery();
    } else {
        qDebug() << "Error removing record " << query.lastError().text();
    }

    return isDelete;
}

bool Database::insertAttributeEntry(const Attribute &attribute)
{
    bool isInsert = false;
    QSqlQuery query;

    query.prepare(QStringLiteral(
        "INSERT INTO %1 (%2, %3, %4, %5, %6, %7) VALUES (:created, :modified, :key, :value, :category, :itemId)").arg(
            TABLE_ATTRIBUTES, CREATED, MODIFIED, KEY, VALUE, CATEGORY, KEY_ITEM_ID));

    QString currentTime = this->getCurrentTime();

    query.bindValue(QStringLiteral(":created"), currentTime);
    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":key"), attribute.getKey());
    query.bindValue(QStringLiteral(":value"), attribute.getValue());
    query.bindValue(QStringLiteral(":category"), attribute.getCategory());
    query.bindValue(QStringLiteral(":itemId"), attribute.getItemId());

    if(query.exec()){
        isInsert = true;
    } else {
        qDebug() << "Error inserting record " << TABLE_ATTRIBUTES;
        qDebug() << query.lastError();
    }

    return isInsert;
}

bool Database::updateAttributeEntry(const Attribute &attribute)
{
    bool isUpdate = false;
    QSqlQuery query;

    query.prepare(QStringLiteral(
        "UPDATE %1 SET %2=:modified, %3=:key, %4=:value, %5=:category WHERE id=:attributeId").arg(
            TABLE_ATTRIBUTES, MODIFIED, KEY, VALUE, CATEGORY));

    QString currentTime = this->getCurrentTime();

    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":key"), attribute.getKey());
    query.bindValue(QStringLiteral(":value"), attribute.getValue());
    query.bindValue(QStringLiteral(":category"), attribute.getCategory());
    query.bindValue(QStringLiteral(":itemId"), attribute.getItemId());

    query.bindValue(QStringLiteral(":attributeId"), attribute.getId());

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
        QStringLiteral("DELETE FROM %1 WHERE id=:attributeId").arg(TABLE_ATTRIBUTES));
    query.bindValue(QStringLiteral(":attributeId"), attributeId);

    if(query.exec()){
        isDelete = true;
    } else {
        qDebug() << "Error removing record " << query.lastError().text();
    }

    return isDelete;
}

bool Database::insertEventEntry(const Event &event)
{
    bool isInsert = false;
    QSqlQuery query;

    query.prepare(QStringLiteral(
        "INSERT INTO %1 ( %2, %3, %4, %5, %6, %7, %8, %9, %10) VALUES "
        "(:created, :modified, :date, :event, :cost, :odometer, :category, :comment, :itemId)").arg(
            TABLE_EVENTS, CREATED, MODIFIED, DATE, EVENT, COST, ODOMETER, CATEGORY, COMMENT, KEY_ITEM_ID));

    QString currentTime = this->getCurrentTime();

    query.bindValue(QStringLiteral(":created"), currentTime);
    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":date"), event.getDate());
    query.bindValue(QStringLiteral(":event"), event.getEvent());
    query.bindValue(QStringLiteral(":cost"), event.getCost());
    query.bindValue(QStringLiteral(":odometer"), event.getOdometer());
    query.bindValue(QStringLiteral(":category"), event.getCategory());
    query.bindValue(QStringLiteral(":comment"), event.getComment());
    query.bindValue(QStringLiteral(":itemId"), event.getItemId());

    if(query.exec()){
        isInsert = true;
    } else {
        qDebug() << "Error inserting record " << TABLE_EVENTS;
        qDebug() << query.lastError();
        qDebug() << query.lastQuery();
    }

    return isInsert;
}

bool Database::updateEventEntry(const Event &event)
{
    bool isUpdate = false;
    QSqlQuery query;

    query.prepare(QStringLiteral(
        "UPDATE %1 SET %2=:modified, %3=:date, %4=:event, %5=:cost, %6=:odometer, %7=:category, %8=:comment "
        "WHERE id=:eventId").arg(TABLE_EVENTS, MODIFIED, DATE, EVENT, COST, ODOMETER, CATEGORY, COMMENT));

    QString currentTime = this->getCurrentTime();

    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":date"), event.getDate());
    query.bindValue(QStringLiteral(":event"), event.getEvent());
    query.bindValue(QStringLiteral(":cost"), event.getCost());
    query.bindValue(QStringLiteral(":odometer"), event.getOdometer());
    query.bindValue(QStringLiteral(":category"), event.getCategory());
    query.bindValue(QStringLiteral(":comment"), event.getComment());

    query.bindValue(QStringLiteral(":eventId"), event.getId());

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
        QStringLiteral("DELETE FROM %1 WHERE id=:eventId").arg(TABLE_EVENTS));
    query.bindValue(QStringLiteral(":eventId"), id);

    if(query.exec()){
        isDelete = true;
    } else {
        qDebug() << "Error removing record " << query.lastError().text();
    }

    return isDelete;
}

bool Database::insertNoteEntry(const Note &note)
{
    bool isInsert = false;
    QSqlQuery query;

    query.prepare(QStringLiteral(
        "INSERT INTO %1 (%2, %3, %4, %5, %6) VALUES (:created, :modified, :title, :noteContent, :itemId)").arg(
            TABLE_NOTES, CREATED, MODIFIED, TITLE, NOTE_CONTENT, KEY_ITEM_ID));

    QString currentTime = this->getCurrentTime();

    query.bindValue(QStringLiteral(":created"), currentTime);
    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":title"), note.getTitle());
    query.bindValue(QStringLiteral(":noteContent"), note.getNoteContent());
    if(note.getItemId()!=-1){
        query.bindValue(QStringLiteral(":itemId"), note.getItemId());
    }

    if(query.exec()){
        isInsert = true;
    } else {
        qDebug() << "Error inserting record " << TABLE_NOTES;
        qDebug() << query.lastError();
        qDebug() << query.lastQuery();
    }

    return isInsert;
}

bool Database::updateNoteEntry(const Note &note)
{
    bool isUpdate = false;
    QSqlQuery query;

    query.prepare(QStringLiteral(
        "UPDATE %1 SET %2=:modified, %3=:title, %4=:noteContent, %5=:itemId WHERE id=:noteId").arg(
            TABLE_NOTES, MODIFIED, TITLE, NOTE_CONTENT, KEY_ITEM_ID));


    query.bindValue(QStringLiteral(":title"), note.getTitle());
    query.bindValue(QStringLiteral(":noteContent"), note.getNoteContent());
    query.bindValue(QStringLiteral(":itemId"), note.getItemId());
    query.bindValue(QStringLiteral(":eventId"), note.getId());

    if(query.exec()){
        isUpdate = true;
    } else {
        qDebug() << "Error updating record " << TABLE_NOTES;
        qDebug() << query.lastError();
        qDebug() << query.lastQuery();
    }

    return isUpdate;
}

bool Database::deleteNoteEntry(int id)
{
    bool isDelete = false;
    QSqlQuery query;

    query.prepare(
        QStringLiteral("DELETE FROM %1 WHERE id=:noteId").arg(TABLE_NOTES));
    query.bindValue(QStringLiteral(":noteId"), id);

    if(query.exec()){
        isDelete = true;
    } else {
        qDebug() << "Error deleting record " << TABLE_NOTES;
        qDebug() << "Error removing record " << query.lastError().text();
    }

    return isDelete;
}

bool Database::insertTaskEntry(const Task &task)
{
    bool isInsert = false;
    QSqlQuery query;

    query.prepare(QStringLiteral(
        "INSERT INTO %1 ( %2, %3, %4, %5, %6, %7, %8) VALUES "
        "(:created, :modified, :dueDate, :status, :title, :description, :itemId)").arg(
            TABLE_TASKS, CREATED, MODIFIED, DUE_DATE, STATUS, TITLE, DESCRIPTION, KEY_ITEM_ID));

    QString currentTime = this->getCurrentTime();

    query.bindValue(QStringLiteral(":created"), currentTime);
    query.bindValue(QStringLiteral(":modified"), currentTime);
    query.bindValue(QStringLiteral(":dueDate"), task.getDueDate());
    query.bindValue(QStringLiteral(":status"), task.getStatus());
    query.bindValue(QStringLiteral(":title"), task.getTitle());
    query.bindValue(QStringLiteral(":description"), task.getDescription());
    if(task.getItemId()!=-1){
        query.bindValue(QStringLiteral(":itemId"), task.getItemId());
    }

    if(query.exec()){
        isInsert = true;
    } else {
        qDebug() << "Error inserting record " << TABLE_TASKS;
        qDebug() << query.lastError();
        qDebug() << query.lastQuery();
    }

    return isInsert;
}

bool Database::updateTaskEntry(const Task &task)
{
    bool isUpdate = false;
    QSqlQuery query;

    query.prepare(QStringLiteral(
        "UPDATE %1 SET %2=:modified, %3=:dueDate, %4=:status, %5=:title, %6=:description, %7=:itemId WHERE id=:taskId").arg(
            TABLE_TASKS, MODIFIED, DUE_DATE, STATUS, TITLE, DESCRIPTION, KEY_ITEM_ID));

    QString currentTime = this->getCurrentTime();

    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":dueDate"), task.getDueDate());
    query.bindValue(QStringLiteral(":status"), task.getStatus());
    query.bindValue(QStringLiteral(":title"), task.getTitle());
    query.bindValue(QStringLiteral(":description"), task.getDescription());
    query.bindValue(QStringLiteral(":itemId"), task.getItemId());

    query.bindValue(QStringLiteral(":taskId"), task.getId());

    if(query.exec()){
        isUpdate = true;
    } else {
        qDebug() << "Error updating record " << TABLE_TASKS;
        qDebug() << query.lastError();
        qDebug() << query.lastQuery();
    }

    return isUpdate;
}

bool Database::deleteTaskEntry(int id)
{
    bool isDelete = false;
    QSqlQuery query;

    query.prepare(
        QStringLiteral("DELETE FROM %1 WHERE id=:taskId").arg(TABLE_TASKS));
    query.bindValue(QStringLiteral(":taskId"), id);

    if(query.exec()){
        isDelete = true;
    } else {
        qDebug() << "Error inserting record " << TABLE_TASKS;
        qDebug() << "Error removing record " << query.lastError().text();
    }

    return isDelete;
}

bool Database::initializeSchema()
{
    bool isSchemaCreate = false;
    QSqlQuery query;

    const QString queryItems = QStringLiteral("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "%2 DATE NOT NULL, "    //CREATED
        "%3 DATE NOT NULL, "    //MODIFIED
        "%4 TEXT NOT NULL, "    //NAME
        "%5 TEXT, "             //MAKE
        "%6 TEXT, "             //MODEL
        "%7 INT, "              //YEAR
        "%8 TEXT NOT NULL, "    //TYPE
        "%9 BOOLEAN NOT NULL CHECK ( %9 IN (0, 1)), "   //ARCHIVED
        "%10 INT, "             //USER_ORDER
        "%11 INT, "             //FK
        "FOREIGN KEY (%11) REFERENCES %1 (id) "
        "ON DELETE CASCADE ON UPDATE CASCADE)").arg(
            TABLE_ITEMS, CREATED, MODIFIED, NAME, MAKE, MODEL, YEAR, TYPE, ARCHIVED, USER_ORDER, KEY_ITEM_ID);

    const QString queryAttributes = QStringLiteral("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "%2 DATE NOT NULL, "
        "%3 DATE NOT NULL, "
        "%4 TEXT NOT NULL, "
        "%5 TEXT NOT NULL, "
        "%6 TEXT, "
        "%7 INT NOT NULL, "
        "CONSTRAINT %7 FOREIGN KEY (%7) REFERENCES %8 (id) "
        "ON DELETE CASCADE ON UPDATE CASCADE)").arg(
            TABLE_ATTRIBUTES, CREATED, MODIFIED, KEY, VALUE, CATEGORY, KEY_ITEM_ID, TABLE_ITEMS);

    const QString queryEvents = QStringLiteral("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "%2 DATE NOT NULL, "
        "%3 DATE NOT NULL, "
        "%4 DATE NOT NULL, "
        "%5 TEXT NOT NULL, "
        "%6 REAL, "
        "%7 REAL, "
        "%8 TEXT, "
        "%9 VARCHAR(255), "
        "%10 INT NOT NULL, "
        "CONSTRAINT %10 FOREIGN KEY (%10) REFERENCES %11 (id) "
        "ON DELETE CASCADE ON UPDATE CASCADE)").arg(
            TABLE_EVENTS, CREATED, MODIFIED, DATE, EVENT, COST, ODOMETER, CATEGORY, COMMENT, KEY_ITEM_ID, TABLE_ITEMS);

    const QString queryNotes = QStringLiteral("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "%2 DATE NOT NULL, "    //CREATED
        "%3 DATE NOT NULL, "    //MODIFIED
        "%4 TEXT NOT NULL, "    //TITLE
        "%5 VARCHAR(255), "     //Note Content
        "%6 INT, "              //FK
        "FOREIGN KEY (%6) REFERENCES %7 (id) "  //Notes and tasks do not need to belong to item
        "ON DELETE CASCADE ON UPDATE CASCADE)").arg(
            TABLE_NOTES, CREATED, MODIFIED, TITLE, NOTE_CONTENT, KEY_ITEM_ID, TABLE_ITEMS);

    const QString queryTasks = QStringLiteral("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "%2 DATE NOT NULL, "    //CREATED
        "%3 DATE NOT NULL, "    //MODIFIED
        "%4 DATE NOT NULL, "    //DUE_DATE
        "%7 TEXT NOT NULL, "    //STATUS
        "%5 TEXT NOT NULL, "    //TITLE
        "%6 VARCHAR(255), "     //DESCRIPTION
        "%8 INT, "              //FK
        "FOREIGN KEY (%8) REFERENCES %9 (id) "  //Notes and tasks do not need to belong to item
        "ON DELETE CASCADE ON UPDATE CASCADE)").arg(
            TABLE_TASKS, CREATED, MODIFIED, DUE_DATE, STATUS, TITLE, DESCRIPTION, KEY_ITEM_ID, TABLE_ITEMS);

    if(!query.exec(queryItems)){
        qDebug() << "Failed to create table: " << TABLE_ITEMS;
        qDebug() << query.lastError();
        return false;
    }

    if(!query.exec(queryAttributes)){
        qDebug() << "Failed to create table: " << TABLE_ATTRIBUTES;
        qDebug() << query.lastError();
        return false;
    }

    if(!query.exec(queryEvents)){
        qDebug() << "Failed to create table: " << TABLE_EVENTS;
        qDebug() << query.lastError();
        return false;
    }

    if(!query.exec(queryNotes)){
        qDebug() << "Failed to create table: " << TABLE_NOTES;
        qDebug() << query.lastQuery();
        qDebug() << query.lastError();
        return false;
    }

    if(!query.exec(queryTasks)){
        qDebug() << "Failed to create table: " << TABLE_TASKS;
        qDebug() << query.lastError();
        return false;
    }

    isSchemaCreate = true;

    return isSchemaCreate;
}
