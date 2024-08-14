#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>

#include "databaseSchema.h"
#include "databaseUtils.h"
#include "entryFactory.h"
#include "event.h"

#ifndef EVENT_DATABASE_H
#define EVENT_DATABASE_H

class EventDatabase : public QObject
{
    Q_OBJECT

public:
    explicit EventDatabase(QObject *parent = nullptr);
    ~EventDatabase() override = default;

    Q_INVOKABLE bool insertEntry(const Event *event) const;
    Q_INVOKABLE bool updateEntry(const Event *event) const;
    Q_INVOKABLE bool deleteEntryById(const int id) const;
    Q_INVOKABLE Event *getEntryById(const int id) const;
    Q_INVOKABLE Event *getNewEntry() const;
};

#endif
