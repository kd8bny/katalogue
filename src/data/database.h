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

    bool insertItemEntry(const Item &item);
    bool updateItemEntry(const Item &item);
    bool deleteItemEntry(int id);

    bool insertAttributeEntry(const Attribute &attribute);
    bool updateAttributeEntry(const Attribute &attribute);
    bool deleteAttributeEntry(int id);
    bool updateItemUserOrder(const int id, const int user_order);

    bool insertEventEntry(const Event &event);
    bool updateEventEntry(const Event &event);
    bool deleteEventEntry(int id);

    bool insertNoteEntry(const Note &note);
    bool updateNoteEntry(const Note &note);
    bool deleteNoteEntry(int id);

    bool insertTaskEntry(const Task &task);
    bool updateTaskEntry(const Task &task);
    bool deleteTaskEntry(int id);

    const static QString TABLE_ATTRIBUTES;
    const static QString TABLE_EVENTS;
    const static QString TABLE_ITEMS;
    const static QString TABLE_NOTES;
    const static QString TABLE_TASKS;
    const static QString TABLE_DEFAULTS;

    const static QString ARCHIVED;
    const static QString CATEGORY;
    const static QString COMMENT;
    const static QString COST;
    const static QString CREATED;
    const static QString DATE;
    const static QString GROUP;
    const static QString KEY;
    const static QString LABEL;
    const static QString EVENT;
    const static QString MAKE;
    const static QString MODEL;
    const static QString MODIFIED;
    const static QString NAME;
    const static QString TYPE;
    const static QString USER_ORDER;
    const static QString VALUE;
    const static QString YEAR;
    const static QString ODOMETER;
    const static QString TITLE;
    const static QString NOTE_CONTENT;
    const static QString DESCRIPTION;
    const static QString DUE_DATE;
    const static QString STATUS;

    const static QString KEY_ITEM_ID;

private:
    QSqlDatabase db;

    const QString getCurrentTime();
    bool initializeSchema();

    const int DATABASE_VERSION = 1;
    const QString DATABASE_TYPE = QStringLiteral("QSQLITE");
    const QString DATABASE_NAME = QStringLiteral("katalogue.db");
};

#endif
