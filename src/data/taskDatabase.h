#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>

#include "databaseSchema.h"
#include "databaseUtils.h"
#include "entryFactory.h"
#include "task.h"

#ifndef TASK_DATABASE_H
#define TASK_DATABASE_H

class TaskDatabase final : public QObject
{
    Q_OBJECT

public:
    explicit TaskDatabase(QObject *parent = nullptr);
    ~TaskDatabase() override = default;

    Q_INVOKABLE bool insertEntry(const Task *task) const;
    Q_INVOKABLE bool updateEntry(const Task *task) const;
    Q_INVOKABLE bool deleteEntryById(const int id) const;
    Q_INVOKABLE Task *getEntryById(const int id = 0) const;
};

#endif
