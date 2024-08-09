#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>

#include "databaseSchema.h"
#include "databaseUtils.h"
#include "entries/task.h"

#ifndef TASK_DATABASE_H
#define TASK_DATABASE_H

class TaskDatabase final : public QObject
{
    Q_OBJECT

public:
    explicit TaskDatabase(QObject *parent = nullptr);
    ~TaskDatabase() override;

    bool insertEntry(const Task &task) const;
    bool updateEntry(const Task &task) const;
    bool deleteEntryById(const int id) const;
};

#endif
