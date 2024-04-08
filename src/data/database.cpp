#include "database.h"


const static QString TABLE_ATTRIBUTES = QStringLiteral("attributes");
const static QString TABLE_EVENTS = QStringLiteral("events");
const static QString TABLE_ITEMS = QStringLiteral("items");
const static QString TABLE_NOTES = QStringLiteral("notes");
const static QString TABLE_TASKS = QStringLiteral("tasks");
const static QString TABLE_DEFAULTS = QStringLiteral("defaults");

const static QString ARCHIVED = QStringLiteral("archived");
const static QString CATEGORY = QStringLiteral("category");
const static QString COMMENT = QStringLiteral("comment");
const static QString COST = QStringLiteral("cost");
const static QString CREATED = QStringLiteral("created");
const static QString DATE = QStringLiteral("date");
const static QString GROUP = QStringLiteral("item_group");
const static QString KEY = QStringLiteral("key");
const static QString LABEL = QStringLiteral("label");
const static QString EVENT = QStringLiteral("event");
const static QString MAKE = QStringLiteral("make");
const static QString MODEL = QStringLiteral("model");
const static QString MODIFIED = QStringLiteral("modified");
const static QString NAME = QStringLiteral("name");
const static QString TYPE = QStringLiteral("type");
const static QString VALUE = QStringLiteral("value");
const static QString YEAR = QStringLiteral("year");
const static QString ODOMETER = QStringLiteral("odometer");
const static QString TITLE = QStringLiteral("title");
const static QString NOTE = QStringLiteral("note");
const static QString DESCRIPTION = QStringLiteral("description");
const static QString DUE_DATE = QStringLiteral("due_date");

const static QString KEY_ITEM_ID = QStringLiteral("fk_item_id");


Database::Database(QObject *parent)
    : QObject(parent)
{

}

Database::~Database()
{

}

QString Database::getCurrentTime()
{
    QDateTime dt = QDateTime::currentDateTime();
    dt.setTimeSpec(Qt::UTC);

    QString date = QDateTime::currentDateTime().toString(Qt::ISODate);
    qDebug() << "getCurrentTime:" << date;

    return date;
}

bool Database::connect(QString path)
{
    bool isDBOpen = false;

    db = QSqlDatabase::addDatabase(DATABASE_TYPE);
    db.setDatabaseName(QStringLiteral("%1/%2").arg(path, this -> DATABASE_NAME));

    if(db.open()){
        QSqlQuery query;

        // Check if the database is empty
        query.exec(QStringLiteral("PRAGMA user_version"));
        query.next();
        int db_version = query.value(0).toInt();
        qDebug() << "database version: " << db_version;

        if(db_version == 0){
            qDebug() << "New database, creating tables";

            query.exec(QStringLiteral("PRAGMA user_version = %1").arg(this -> DATABASE_VERSION));
            this->initializeSchema();
        }else if(db_version > this -> DATABASE_VERSION){
            qDebug() << "Database is newer than application";
        }else{
            // Enable ON CASCADE
            query.exec(QStringLiteral("PRAGMA foreign_keys = ON;"));
            isDBOpen = true;
        }
    }

    return isDBOpen;
}

bool Database::insertItemEntry(Item item)
{
    bool isInsert = false;
    QSqlQuery query;

    query.prepare(QStringLiteral("INSERT INTO %1 (%2, %3, %4, %5, %6, %7, %8, %9, %10) "
        "VALUES (:created, :modified, :name, :make, :model, :year, :type, :archived, :parent)").arg(
            ::TABLE_ITEMS, ::CREATED, ::MODIFIED, ::NAME, ::MAKE, ::MODEL, ::YEAR, ::TYPE, ::ARCHIVED, ::KEY_ITEM_ID));

    QString currentTime = this->getCurrentTime();

    query.bindValue(QStringLiteral(":created"), currentTime);
    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":name"), item.getName());
    query.bindValue(QStringLiteral(":make"), item.getMake());
    query.bindValue(QStringLiteral(":model"), item.getModel());
    query.bindValue(QStringLiteral(":year"), item.getYear());
    query.bindValue(QStringLiteral(":type"), item.getType());
    query.bindValue(QStringLiteral(":archived"), item.getArchived());
    if(item.getParent() != -1){
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

bool Database::updateItemEntry(Item item)
{
    bool isUpdate = false;
    QSqlQuery query;

    query.prepare(QStringLiteral("UPDATE %1 SET %2=:modified, %3=:name, %4=:make, %5=:model, "
        "%6=:year, %7=:type, %8=:archived, %9=:parent WHERE id=:id").arg(
            ::TABLE_ITEMS, ::MODIFIED, ::NAME, ::MAKE, ::MODEL, ::YEAR, ::TYPE, ::ARCHIVED, ::KEY_ITEM_ID));

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

    query.prepare(QStringLiteral("DELETE FROM %1 WHERE id=:itemId").arg(::TABLE_ITEMS));
    query.bindValue(QStringLiteral(":itemId"), id);

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

    query.prepare(QStringLiteral(
        "INSERT INTO %1 (%2, %3, %4, %5, %6, %7) VALUES (:created, :modified, :key, :value, :category, :itemId)").arg(
            ::TABLE_ATTRIBUTES, ::CREATED, ::MODIFIED, ::KEY, ::VALUE, ::CATEGORY, ::KEY_ITEM_ID));

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
        qDebug() << "Error inserting record " << ::TABLE_ATTRIBUTES;
        qDebug() << query.lastError();
    }

    return isInsert;
}

bool Database::updateAttributeEntry(Attribute attribute)
{
    bool isUpdate = false;
    QSqlQuery query;

    query.prepare(QStringLiteral(
        "UPDATE %1 SET %2=:modified, %3=:key, %4=:value, %5=:category WHERE id=:attributeId").arg(
            ::TABLE_ATTRIBUTES, ::MODIFIED, ::KEY, ::VALUE, ::CATEGORY));

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
        qDebug() << "Error updating record " << ::TABLE_ATTRIBUTES;
        qDebug() << query.lastError();
    }

    return isUpdate;
}

bool Database::deleteAttributeEntry(int attributeId)
{
    bool isDelete = false;
    QSqlQuery query;

    query.prepare(
        QStringLiteral("DELETE FROM %1 WHERE id=:attributeId").arg(::TABLE_ATTRIBUTES));
    query.bindValue(QStringLiteral(":attributeId"), attributeId);

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

    query.prepare(QStringLiteral(
        "INSERT INTO %1 ( %2, %3, %4, %5, %6, %7, %8, %9, %10) VALUES "
        "(:created, :modified, :date, :event, :cost, :odometer, :category, :comment, :itemId)").arg(
            ::TABLE_EVENTS, ::CREATED, ::MODIFIED, ::DATE, ::EVENT, ::COST, ::ODOMETER, ::CATEGORY, ::COMMENT, ::KEY_ITEM_ID));

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
        qDebug() << "Error inserting record " << ::TABLE_EVENTS;
        qDebug() << query.lastError();
        qDebug() << query.lastQuery();
    }

    return isInsert;
}

bool Database::updateEventEntry(Event event)
{
    bool isUpdate = false;
    QSqlQuery query;

    query.prepare(QStringLiteral(
        "UPDATE %1 SET %2=:modified, %3=:date, %4=:event, %5=:cost, %6=:odometer, %7=:category, %8=:comment"
        "WHERE id=:eventId").arg(
            ::TABLE_EVENTS, ::MODIFIED, ::DATE, ::EVENT, ::COST, ::ODOMETER, ::CATEGORY, ::COMMENT));

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
        qDebug() << "Error updating record " << ::TABLE_EVENTS;
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
        QStringLiteral("DELETE FROM %1 WHERE id=:eventId").arg(::TABLE_EVENTS));
    query.bindValue(QStringLiteral(":eventId"), id);

    if(query.exec()){
        isDelete = true;
    } else {
        qDebug() << "Error removing record " << query.lastError().text();
    }

    return isDelete;
}

bool Database::insertNoteEntry(Note note)
{
    bool isInsert = false;
    QSqlQuery query;

    query.prepare(QStringLiteral(
        "INSERT INTO %1 (%2, %3, %4, %5, %6) VALUES (:created, :modified, :title, :noteContent, :itemId)").arg(
            ::TABLE_NOTES, ::CREATED, ::MODIFIED, ::TITLE, ::NOTE, ::KEY_ITEM_ID));

    QString currentTime = this->getCurrentTime();

    query.bindValue(QStringLiteral(":created"), currentTime);
    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":title"), note.getTitle());
    query.bindValue(QStringLiteral(":noteContent"), note.getNoteContent());
    if(note.getItemId() != -1){
        query.bindValue(QStringLiteral(":itemId"), note.getItemId());
    }

    if(query.exec()){
        isInsert = true;
    } else {
        qDebug() << "Error inserting record " << ::TABLE_NOTES;
        qDebug() << query.lastError();
        qDebug() << query.lastQuery();
    }

    return isInsert;
}

bool Database::updateNoteEntry(Note note)
{
    bool isUpdate = false;
    QSqlQuery query;

    query.prepare(QStringLiteral(
        "UPDATE %1 SET %2=:modified, %3=:title, %4=:noteContent, %5=:itemId WHERE id=:noteId").arg(
            ::TABLE_NOTES, ::MODIFIED, ::TITLE, ::NOTE, ::KEY_ITEM_ID));


    query.bindValue(QStringLiteral(":title"), note.getTitle());
    query.bindValue(QStringLiteral(":noteContent"), note.getNoteContent());
    query.bindValue(QStringLiteral(":itemId"), note.getItemId());

    query.bindValue(QStringLiteral(":eventId"), note.getId());

    if(query.exec()){
        isUpdate = true;
    } else {
        qDebug() << "Error updating record " << ::TABLE_NOTES;
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
        QStringLiteral("DELETE FROM %1 WHERE id=:noteId").arg(::TABLE_NOTES));
    query.bindValue(QStringLiteral(":noteId"), id);

    if(query.exec()){
        isDelete = true;
    } else {
        qDebug() << "Error deleting record " << ::TABLE_NOTES;
        qDebug() << "Error removing record " << query.lastError().text();
    }

    return isDelete;
}

bool Database::insertTaskEntry(Task &task)
{
    bool isInsert = false;
    QSqlQuery query;

    query.prepare(QStringLiteral(
        "INSERT INTO %1 ( %2, %3, %4, %5, %6, %7, %8, %9, %10) VALUES "
        "(:created, :modified, :title, :desc, :dueDate, :itemId)").arg(
            ::TABLE_TASKS, ::CREATED, ::MODIFIED, ::TITLE, ::DESCRIPTION, ::DUE_DATE, ::KEY_ITEM_ID));

    QString currentTime = this->getCurrentTime();

    query.bindValue(QStringLiteral(":created"), currentTime);
    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":title"), task.getTitle());
    query.bindValue(QStringLiteral(":desc"), task.getDescription());
    query.bindValue(QStringLiteral(":dueDate"), task.getDueDate());
    if(task.getItemId() != -1){
        query.bindValue(QStringLiteral(":itemId"), task.getItemId());
    }

    if(query.exec()){
        isInsert = true;
    } else {
        qDebug() << "Error inserting record " << ::TABLE_TASKS;
        qDebug() << query.lastError();
        qDebug() << query.lastQuery();
    }

    return isInsert;
}

bool Database::updateTaskEntry(Task task)
{
    bool isUpdate = false;
    QSqlQuery query;

    query.prepare(QStringLiteral(
        "UPDATE %1 SET %2=:modified, %3=:title, %4=:desc, %5=:dueDate, %6=:itemId WHERE id=:taskId").arg(
            ::TABLE_TASKS, ::MODIFIED, ::TITLE, ::DESCRIPTION, ::DUE_DATE, ::KEY_ITEM_ID));

    QString currentTime = this->getCurrentTime();

    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":title"), task.getTitle());
    query.bindValue(QStringLiteral(":desc"), task.getDescription());
    query.bindValue(QStringLiteral(":dueDate"), task.getDueDate());
    query.bindValue(QStringLiteral(":itemId"), task.getItemId());

    query.bindValue(QStringLiteral(":taskId"), task.getId());

    if(query.exec()){
        isUpdate = true;
    } else {
        qDebug() << "Error updating record " << ::TABLE_TASKS;
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
        QStringLiteral("DELETE FROM %1 WHERE id=:taskId").arg(::TABLE_TASKS));
    query.bindValue(QStringLiteral(":taskId"), id);

    if(query.exec()){
        isDelete = true;
    } else {
        qDebug() << "Error inserting record " << ::TABLE_TASKS;
        qDebug() << "Error removing record " << query.lastError().text();
    }

    return isDelete;
}

bool Database::initializeSchema()
{
    bool isSchemaCreate = false;
    QSqlQuery query;

    // const QString queryItems = QStringLiteral("CREATE TABLE " ::TABLE_ITEMS
    //     " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
    //     CREATED     " DATE NOT NULL, "
    //     MODIFIED    " DATE NOT NULL, "
    //     NAME        " TEXT NOT NULL, "
    //     MAKE        " TEXT, "
    //     MODEL       " TEXT, "
    //     YEAR        " INT, "
    //     TYPE        " TEXT NOT NULL, "
    //     ARCHIVED    " BOOLEAN NOT NULL CHECK (" ARCHIVED " IN (0, 1)), "
    //     KEY_ITEM_ID " INT, "
    //     "FOREIGN KEY (" KEY_ITEM_ID ") REFERENCES " ::TABLE_ITEMS "(id) "
    //     "ON DELETE CASCADE ON UPDATE CASCADE)");

    const QString queryItems = QStringLiteral("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "%2 DATE NOT NULL, "
        "%3 DATE NOT NULL, "
        "%4 TEXT NOT NULL, "
        "%5 TEXT, "
        "%6 TEXT, "
        "%7 INT, "
        "%8 TEXT NOT NULL, "
        "%9 BOOLEAN NOT NULL CHECK ( %9 IN (0, 1)), "
        "%10 INT, "
        "FOREIGN KEY (%10) REFERENCES %1 (id) "
        "ON DELETE CASCADE ON UPDATE CASCADE)").arg(
            ::TABLE_ITEMS, ::CREATED, ::MODIFIED, ::NAME, ::MAKE, ::MODEL,
            ::YEAR, ::TYPE, ::ARCHIVED, ::KEY_ITEM_ID);

    // const QString queryAttributes = QStringLiteral("CREATE TABLE " ::TABLE_ATTRIBUTES
    //     " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
    //     CREATED     " DATE NOT NULL, "
    //     MODIFIED    " DATE NOT NULL, "
    //     KEY           " TEXT NOT NULL, "
    //     VALUE         " TEXT NOT NULL, "
    //     CATEGORY      " TEXT, "
    //     KEY_ITEM_ID   " INT NOT NULL, "
    //     "CONSTRAINT itemId FOREIGN KEY (" KEY_ITEM_ID ") REFERENCES " ::TABLE_ITEMS "(id) "
    //     "ON DELETE CASCADE ON UPDATE CASCADE)");

    const QString queryAttributes = QStringLiteral("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "%2 DATE NOT NULL, "
        "%3 DATE NOT NULL, "
        "%4 TEXT NOT NULL, "
        "%5 TEXT NOT NULL, "
        "%6 TEXT, "
        "%7 INT NOT NULL, "
        "CONSTRAINT itemId FOREIGN KEY (%7) REFERENCES %1 (id) "
        "ON DELETE CASCADE ON UPDATE CASCADE)").arg(
            ::TABLE_ATTRIBUTES, ::CREATED, ::MODIFIED, ::KEY, ::VALUE, ::CATEGORY, ::KEY_ITEM_ID);

    // const QString queryEvents = QStringLiteral("CREATE TABLE " ::TABLE_EVENTS
    //     " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
    //     CREATED     " DATE NOT NULL, "
    //     MODIFIED    " DATE NOT NULL, "
    //     DATE        " DATE NOT NULL, "
    //     EVENT       " TEXT NOT NULL, "
    //     COST        " REAL, "
    //     ODOMETER    " REAL, "
    //     CATEGORY    " TEXT, "
    //     COMMENT     " VARCHAR(255), "
    //     KEY_ITEM_ID " INT NOT NULL, "
    //     "CONSTRAINT itemId FOREIGN KEY (" KEY_ITEM_ID ") REFERENCES " ::TABLE_ITEMS "(id) "
    //     "ON DELETE CASCADE ON UPDATE CASCADE)");

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
        "CONSTRAINT itemId FOREIGN KEY (%10) REFERENCES %1 (id) "
        "ON DELETE CASCADE ON UPDATE CASCADE)").arg(
            ::TABLE_EVENTS, ::CREATED, ::MODIFIED, ::DATE, ::EVENT, ::COST,
            ::ODOMETER, ::CATEGORY, ::COMMENT, ::KEY_ITEM_ID);

    // const QString queryNotes = QStringLiteral("CREATE TABLE " ::TABLE_NOTES
    //     " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
    //     CREATED     " DATE NOT NULL, "
    //     MODIFIED    " DATE NOT NULL, "
    //     TITLE       " TEXT NOT NULL, "
    //     NOTE        " VARCHAR(255), "
    //     KEY_ITEM_ID " INT, "
    //     "FOREIGN KEY (" KEY_ITEM_ID ") REFERENCES " ::TABLE_ITEMS "(id) "
    //     "ON DELETE CASCADE ON UPDATE CASCADE)");

    const QString queryNotes = QStringLiteral("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "%2 DATE NOT NULL, "
        "%3 DATE NOT NULL, "
        "%4 TEXT NOT NULL, "
        "%5 VARCHAR(255), "
        "%6 INT, "
        "FOREIGN KEY (%6) REFERENCES %1 (id) "
        "ON DELETE CASCADE ON UPDATE CASCADE)").arg(
            ::TABLE_NOTES, ::CREATED, ::MODIFIED, ::TITLE, ::NOTE, ::KEY_ITEM_ID);

    // const QString queryTasks = QStringLiteral("CREATE TABLE " ::TABLE_TASKS
    //     " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
    //     CREATED     " DATE NOT NULL, "
    //     MODIFIED    " DATE NOT NULL, "
    //     DUE_DATE    " DATE NOT NULL, "
    //     TITLE       " TEXT NOT NULL, "
    //     DESCRIPTION " VARCHAR(255), "
    //     KEY_ITEM_ID " INT, "
    //     "FOREIGN KEY (" KEY_ITEM_ID ") REFERENCES " ::TABLE_ITEMS "(id) "
    //     "ON DELETE CASCADE ON UPDATE CASCADE)");

    const QString queryTasks = QStringLiteral("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "%2 DATE NOT NULL, "
        "%3 DATE NOT NULL, "
        "%4 DATE NOT NULL, "
        "%5 TEXT NOT NULL, "
        "%6 VARCHAR(255), "
        "%7 INT, "
        "FOREIGN KEY (%7) REFERENCES %1 (id) "
        "ON DELETE CASCADE ON UPDATE CASCADE)").arg(
            ::TABLE_TASKS, ::CREATED, ::MODIFIED, ::DUE_DATE, ::TITLE, ::DESCRIPTION, ::KEY_ITEM_ID);

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

    if(!query.exec(queryNotes)){
        qDebug() << query.lastError();
        return false;
    }

    if(!query.exec(queryTasks)){
        qDebug() << query.lastError();
        return false;
    }

    isSchemaCreate = true;

    return isSchemaCreate;
}
