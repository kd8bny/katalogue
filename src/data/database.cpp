#include "database.h"

Database::Database(QObject *parent)
    : QObject(parent)
{

}

Database::~Database()
{

}

QString Database::getCurrentTime()
{
    // auto currentTime = std::chrono::system_clock::now();
    // std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
    // std::tm* currentTime_tm = std::gmtime(&currentTime_t);
    // qDebug() << std::asctime(currentTime_tm);

    QDateTime dt = QDateTime::currentDateTime();
    dt.setTimeSpec(Qt::UTC);

    QString date = QDateTime::currentDateTime().toString(Qt::ISODate);
    qDebug() << date;

    return date;
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

    query.prepare(QString("INSERT INTO %1 (%2, %3, %4, %5, %6, %7, %8, %9, %10) "
        "VALUES (:created, :modified, :name, :make, :model, :year, :type, :archived, :parent)").arg(
            TABLE_ITEMS, CREATED, MODIFIED, NAME, MAKE, MODEL, YEAR, TYPE, ARCHIVED, KEY_ITEM_ID));

    QString currentTime = this->getCurrentTime();

    query.bindValue(":created", currentTime);
    query.bindValue(":modified", currentTime);

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

    query.prepare(QString("UPDATE %1 SET %2=:modified, %3=:name, %4=:make, %5=:model, "
        "%6=:year, %7=:type, %8=:archived, %9=:parent WHERE id=:id").arg(
            TABLE_ITEMS, MODIFIED, NAME, MAKE, MODEL, YEAR, TYPE, ARCHIVED, KEY_ITEM_ID));

    QString currentTime = this->getCurrentTime();

    query.bindValue(":modified", currentTime);

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
        "INSERT INTO %1 (%2, %3, %4, %5, %6, %7) VALUES (:created, :modified, :key, :value, :category, :itemId)").arg(
            TABLE_ATTRIBUTES, CREATED, MODIFIED, KEY, VALUE, CATEGORY, KEY_ITEM_ID));

    QString currentTime = this->getCurrentTime();

    query.bindValue(":created", currentTime);
    query.bindValue(":modified", currentTime);

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
        "UPDATE %1 SET %2=:modified, %3=:key, %4=:value, %5=:category WHERE id=:attributeId").arg(
            TABLE_ATTRIBUTES, MODIFIED, KEY, VALUE, CATEGORY));

    QString currentTime = this->getCurrentTime();

    query.bindValue(":modified", currentTime);

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
        "INSERT INTO %1 ( %2, %3, %4, %5, %6, %7, %8, %9, %10) VALUES "
        "(:created, :modified, :date, :event, :cost, :odometer, :category, :comment, :itemId)").arg(
            TABLE_EVENTS, CREATED, MODIFIED, DATE, EVENT, COST, ODOMETER, CATEGORY, COMMENT, KEY_ITEM_ID));

    QString currentTime = this->getCurrentTime();

    query.bindValue(":created", currentTime);
    query.bindValue(":modified", currentTime);

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
        "UPDATE %1 SET %2=:modified, %3=:date, %4=:event, %5=:cost, %6=:odometer, %7=:category, %8=:comment"
        "WHERE id=:eventId").arg(
            TABLE_EVENTS, MODIFIED, DATE, EVENT, COST, ODOMETER, CATEGORY, COMMENT));

    QString currentTime = this->getCurrentTime();

    query.bindValue(":modified", currentTime);

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

bool Database::insertNoteEntry(Note note)
{
    bool isInsert = false;
    QSqlQuery query;

    query.prepare(QString(
        "INSERT INTO %1 (%2, %3, %4, %5, %6) VALUES (:created, :modified, :title, :noteContent, :itemId)").arg(
            TABLE_NOTES, CREATED, MODIFIED, TITLE, NOTE, KEY_ITEM_ID));

    QString currentTime = this->getCurrentTime();

    query.bindValue(":created", currentTime);
    query.bindValue(":modified", currentTime);

    query.bindValue(":title", note.getTitle());
    query.bindValue(":noteContent", note.getNoteContent());
    if(note.getItemId() != -1){
        query.bindValue(":itemId", note.getItemId());
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

bool Database::updateNoteEntry(Note note)
{
    bool isUpdate = false;
    QSqlQuery query;

    query.prepare(QString(
        "UPDATE %1 SET %2=:modified, %3=:title, %4=:noteContent, %5=:itemId WHERE id=:noteId").arg(
            TABLE_NOTES, MODIFIED, TITLE, NOTE, KEY_ITEM_ID));


    query.bindValue(":title", note.getTitle());
    query.bindValue(":noteContent", note.getNoteContent());
    query.bindValue(":itemId", note.getItemId());

    query.bindValue(":eventId", note.getId());

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
        QString("DELETE FROM %1 WHERE id=:noteId").arg(TABLE_NOTES));
    query.bindValue(":noteId", id);

    if(query.exec()){
        isDelete = true;
    } else {
        qDebug() << "Error deleting record " << TABLE_NOTES;
        qDebug() << "Error removing record " << query.lastError().text();
    }

    return isDelete;
}

bool Database::insertTaskEntry(Task task)
{
    bool isInsert = false;
    QSqlQuery query;

    query.prepare(QString(
        "INSERT INTO %1 ( %2, %3, %4, %5, %6, %7, %8, %9, %10) VALUES "
        "(:created, :modified, :title, :desc, :dueDate, :itemId)").arg(
            TABLE_TASKS, CREATED, MODIFIED, TITLE, DESCRIPTION, DUE_DATE, KEY_ITEM_ID));

    QString currentTime = this->getCurrentTime();

    query.bindValue(":created", currentTime);
    query.bindValue(":modified", currentTime);

    query.bindValue(":title", task.getTitle());
    query.bindValue(":desc", task.getDescription());
    query.bindValue(":dueDate", task.getDueDate());
    if(task.getItemId() != -1){
        query.bindValue(":itemId", task.getItemId());
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

bool Database::updateTaskEntry(Task task)
{
    bool isUpdate = false;
    QSqlQuery query;

    query.prepare(QString(
        "UPDATE %1 SET %2=:modified, %3=:title, %4=:desc, %5=:dueDate, %6=:itemId WHERE id=:taskId").arg(
            TABLE_TASKS, MODIFIED, TITLE, DESCRIPTION, DUE_DATE, KEY_ITEM_ID));

    QString currentTime = this->getCurrentTime();

    query.bindValue(":modified", currentTime);

    query.bindValue(":title", task.getTitle());
    query.bindValue(":desc", task.getDescription());
    query.bindValue(":dueDate", task.getDueDate());
    query.bindValue(":itemId", task.getItemId());

    query.bindValue(":taskId", task.getId());

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
        QString("DELETE FROM %1 WHERE id=:taskId").arg(TABLE_TASKS));
    query.bindValue(":taskId", id);

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

    const QString queryItems = "CREATE TABLE " TABLE_ITEMS
        " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        CREATED     " DATE NOT NULL, "
        MODIFIED    " DATE NOT NULL, "
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
        CREATED     " DATE NOT NULL, "
        MODIFIED    " DATE NOT NULL, "
        KEY           " TEXT NOT NULL, "
        VALUE         " TEXT NOT NULL, "
        CATEGORY      " TEXT, "
        KEY_ITEM_ID   " INT NOT NULL, "
        "CONSTRAINT itemId FOREIGN KEY (" KEY_ITEM_ID ") REFERENCES " TABLE_ITEMS "(id) "
        "ON DELETE CASCADE ON UPDATE CASCADE)";

    const QString queryEvents = "CREATE TABLE " TABLE_EVENTS
        " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        CREATED     " DATE NOT NULL, "
        MODIFIED    " DATE NOT NULL, "
        DATE        " DATE NOT NULL, "
        EVENT       " TEXT NOT NULL, "
        COST        " REAL, "
        ODOMETER    " REAL, "
        CATEGORY    " TEXT, "
        COMMENT     " VARCHAR(255), "
        KEY_ITEM_ID " INT NOT NULL, "
        "CONSTRAINT itemId FOREIGN KEY (" KEY_ITEM_ID ") REFERENCES " TABLE_ITEMS "(id) "
        "ON DELETE CASCADE ON UPDATE CASCADE)";

    const QString queryNotes = "CREATE TABLE " TABLE_NOTES
        " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        CREATED     " DATE NOT NULL, "
        MODIFIED    " DATE NOT NULL, "
        TITLE       " TEXT NOT NULL, "
        NOTE        " VARCHAR(255), "
        KEY_ITEM_ID " INT, "
        "FOREIGN KEY (" KEY_ITEM_ID ") REFERENCES " TABLE_ITEMS "(id) "
        "ON DELETE CASCADE ON UPDATE CASCADE)";

    const QString queryTasks = "CREATE TABLE " TABLE_TASKS
        " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
        CREATED     " DATE NOT NULL, "
        MODIFIED    " DATE NOT NULL, "
        DUE_DATE    " DATE NOT NULL, "
        TITLE       " TEXT NOT NULL, "
        DESCRIPTION " VARCHAR(255), "
        KEY_ITEM_ID " INT, "
        "FOREIGN KEY (" KEY_ITEM_ID ") REFERENCES " TABLE_ITEMS "(id) "
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
