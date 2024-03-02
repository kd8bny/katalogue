#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QFile>
#include <QDate>
#include <QDebug>
#include <QUuid>
#include <QDateTime>

#include "attribute.h"
#include "event.h"
#include "item.h"
#include "note.h"
#include "task.h"

#ifndef DATABASE_H
#define DATABASE_H

#define DATABASE_TYPE "QSQLITE"
#define DATABASE_NAME "katalogue.db"
#define USER_VERSION 1

#define TABLE_ATTRIBUTES "attributes"
#define TABLE_EVENTS "events"
#define TABLE_ITEMS "items"
#define TABLE_NOTES "notes"
#define TABLE_TASKS "tasks"
#define TABLE_DEFAULTS "defaults"

#define KEY_ITEM_ID "fk_item_id"

#define ARCHIVED "archived"
#define CATEGORY "category"
#define COMMENT "comment"
#define COST "cost"
#define CREATED "created"
#define DATE "date"
#define GROUP "item_group"
#define KEY "key"
#define LABEL "label"
#define EVENT "event"
#define MAKE "make"
#define MODEL "model"
#define MODIFIED "modified"
#define NAME "name"
#define TYPE "type"
#define VALUE "value"
#define YEAR "year"
#define ODOMETER "odometer"
#define TITLE "title"
#define NOTE "note"
#define DESCRIPTION "description"
#define DUE_DATE "due_date"


class Database : public QObject
{
    Q_OBJECT

public:
    explicit Database(QObject *parent = nullptr);
    ~Database();
    bool connect(QString path);

    bool insertItemEntry(Item item);
    bool updateItemEntry(Item item);
    bool deleteItemEntry(int id);

    bool insertAttributeEntry(Attribute attribute);
    bool updateAttributeEntry(Attribute attribute);
    bool deleteAttributeEntry(int id);

    bool insertEventEntry(Event event);
    bool updateEventEntry(Event event);
    bool deleteEventEntry(int id);

    bool insertNoteEntry(Note note);
    bool updateNoteEntry(Note note);
    bool deleteNoteEntry(int id);

    bool insertTaskEntry(Task task);
    bool updateTaskEntry(Task task);
    bool deleteTaskEntry(int id);

private:
    QSqlDatabase db;

    QString getCurrentTime();
    bool initializeSchema();
};

#endif
