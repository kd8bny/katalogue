#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>

#include "databaseSchema.h"
#include "databaseUtils.h"
#include "entries/event.h"

#ifndef EVENT_DATABASE_H
#define EVENT_DATABASE_H

class EventDatabase : public QObject
{
    Q_OBJECT

public:
    explicit EventDatabase(QObject *parent = nullptr);
    ~EventDatabase() override;

    bool insertEntry(const Event &event) const;
    bool updateEntry(const Event &event) const;
    bool deleteEntryById(const int id) const;
};

#endif
