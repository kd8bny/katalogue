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
#include <QString>

#include "attribute.h"
#include "event.h"
#include "item.h"
#include "note.h"
#include "task.h"

#ifndef DATABASE_H
#define DATABASE_H

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

    bool insertTaskEntry(Task &task);
    bool updateTaskEntry(Task task);
    bool deleteTaskEntry(int id);
    const QString KEY = QStringLiteral("key");

private:
    QSqlDatabase db;

    QString getCurrentTime();
    bool initializeSchema();

    int DATABASE_VERSION = 1;
    const QString DATABASE_TYPE = QStringLiteral("QSQLITE");
    const QString DATABASE_NAME = QStringLiteral("katalogue.db");

    const QString TABLE_ATTRIBUTES = QStringLiteral("attributes");
    const QString TABLE_EVENTS = QStringLiteral("events");
    const QString TABLE_ITEMS = QStringLiteral("items");
    const QString TABLE_NOTES = QStringLiteral("notes");
    const QString TABLE_TASKS = QStringLiteral("tasks");
    const QString TABLE_DEFAULTS = QStringLiteral("defaults");

    const QString KEY_ITEM_ID = QStringLiteral("fk_item_id");

    const QString ARCHIVED = QStringLiteral("archived");
    const QString CATEGORY = QStringLiteral("category");
    const QString COMMENT = QStringLiteral("comment");
    const QString COST = QStringLiteral("cost");
    const QString CREATED = QStringLiteral("created");
    const QString DATE = QStringLiteral("date");
    const QString GROUP = QStringLiteral("item_group");
    const QString KEY2 = QStringLiteral("key");
    const QString LABEL = QStringLiteral("label");
    const QString EVENT = QStringLiteral("event");
    const QString MAKE = QStringLiteral("make");
    const QString MODEL = QStringLiteral("model");
    const QString MODIFIED = QStringLiteral("modified");
    const QString NAME = QStringLiteral("name");
    const QString TYPE = QStringLiteral("type");
    const QString VALUE = QStringLiteral("value");
    const QString YEAR = QStringLiteral("year");
    const QString ODOMETER = QStringLiteral("odometer");
    const QString TITLE = QStringLiteral("title");
    const QString NOTE = QStringLiteral("note");
    const QString DESCRIPTION = QStringLiteral("description");
    const QString DUE_DATE = QStringLiteral("due_date");
};

#endif
