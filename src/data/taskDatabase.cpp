#include "taskDatabase.h"

TaskDatabase::TaskDatabase(QObject *parent)
    : QObject(parent)
{
}

bool TaskDatabase::insertEntry(const Task *task) const
{
    QSqlQuery query;

    query.prepare(
        QStringLiteral("INSERT INTO %1 ( %2, %3, %4, %5, %6, %7, %8) VALUES "
                       "(:created, :modified, :dueDate, :status, :title, :description, :itemId)")
            .arg(DatabaseSchema::TABLE_TASKS, DatabaseSchema::CREATED, DatabaseSchema::MODIFIED,
                 DatabaseSchema::DUE_DATE, DatabaseSchema::STATUS, DatabaseSchema::TITLE, DatabaseSchema::DESCRIPTION,
                 DatabaseSchema::KEY_ITEM_ID));

    QString currentTime = DatabaseUtils::getCurrentDateTime();

    query.bindValue(QStringLiteral(":created"), currentTime);
    query.bindValue(QStringLiteral(":modified"), currentTime);
    query.bindValue(QStringLiteral(":dueDate"), task->getDueDate());
    query.bindValue(QStringLiteral(":status"), task->getStatus());
    query.bindValue(QStringLiteral(":title"), task->getTitle());
    query.bindValue(QStringLiteral(":description"), task->getDescription());
    if (task->getItemId() != 0)
    {
        query.bindValue(QStringLiteral(":itemId"), task->getItemId());
    }

    if (!query.exec())
    {
        qDebug() << "Error inserting record " << DatabaseSchema::TABLE_TASKS;
        qDebug() << query.lastQuery() << query.lastError();
        return false;
    }

    return true;
}

bool TaskDatabase::updateEntry(const Task *task) const
{
    QSqlQuery query;

    query.prepare(
        QStringLiteral("UPDATE %1 SET %2=:modified, %3=:dueDate, %4=:status, %5=:title, %6=:description, %7=:itemId "
                       "WHERE id=:taskId")
            .arg(DatabaseSchema::TABLE_TASKS, DatabaseSchema::MODIFIED, DatabaseSchema::DUE_DATE,
                 DatabaseSchema::STATUS, DatabaseSchema::TITLE, DatabaseSchema::DESCRIPTION,
                 DatabaseSchema::KEY_ITEM_ID));

    QString currentTime = DatabaseUtils::getCurrentDateTime();

    query.bindValue(QStringLiteral(":modified"), currentTime);
    qDebug() << task->getDueDate();
    query.bindValue(QStringLiteral(":dueDate"), task->getDueDate());
    query.bindValue(QStringLiteral(":status"), task->getStatus());
    query.bindValue(QStringLiteral(":title"), task->getTitle());
    query.bindValue(QStringLiteral(":description"), task->getDescription());
    if (task->getItemId() != 0)
    {
        query.bindValue(QStringLiteral(":itemId"), task->getItemId());
    }

    query.bindValue(QStringLiteral(":taskId"), task->getId());

    if (!query.exec())
    {
        qDebug() << "Error updating record " << DatabaseSchema::TABLE_TASKS;
        qDebug() << query.lastQuery() << query.lastError();
        return false;
    }

    return true;
}

bool TaskDatabase::deleteEntryById(const int id) const
{
    QSqlQuery query;

    query.prepare(QStringLiteral("DELETE FROM %1 WHERE id=:taskId").arg(DatabaseSchema::TABLE_TASKS));
    query.bindValue(QStringLiteral(":taskId"), id);

    if (!query.exec())
    {
        qDebug() << "Error inserting record " << DatabaseSchema::TABLE_TASKS;
        qDebug() << query.lastQuery() << query.lastError();
        return false;
    }

    return true;
}

Task *TaskDatabase::getEntryById(const int id) const
{
    EntryFactory entryFactory;
    Task *task = entryFactory.createTask();

    if (id == 0)
    {
        return task;
    }

    QSqlQuery query;

    query.prepare(
        QStringLiteral("SELECT id, %1, %2, %3, %4, %5, %6, %7 FROM %8 WHERE id=:id")
            .arg(DatabaseSchema::CREATED, DatabaseSchema::MODIFIED, DatabaseSchema::DUE_DATE, DatabaseSchema::STATUS,
                 DatabaseSchema::TITLE, DatabaseSchema::DESCRIPTION, DatabaseSchema::KEY_ITEM_ID,
                 DatabaseSchema::TABLE_TASKS));

    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec())
    {
        qDebug() << "Error getting task entry " << query.lastError();
        qDebug() << query.lastQuery() << query.lastError();
        return nullptr;
    }
    query.next();

    task->setId(id);
    task->setCreatedDate(query.value(1).toString());
    task->setModifiedDate(query.value(2).toString());
    task->setDueDate(query.value(3).toString());
    task->setStatus(query.value(4).toString());
    task->setTitle(query.value(5).toString());
    task->setDescription(query.value(5).toString());
    task->setItemId(query.value(6).toInt());

    return task;
}
