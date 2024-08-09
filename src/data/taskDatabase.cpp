#include "taskDatabase.h"

TaskDatabase::TaskDatabase(QObject *parent)
    : QObject(parent)
{
}

TaskDatabase::~TaskDatabase() = default;

bool TaskDatabase::insertEntry(const Task &task) const
{
    QSqlQuery query;

    query.prepare(QStringLiteral("INSERT INTO %1 ( %2, %3, %4, %5, %6, %7, %8) VALUES "
                                 "(:created, :modified, :dueDate, :status, :title, :description, :itemId)")
                      .arg(DatabaseSchema::TABLE_TASKS, DatabaseSchema::CREATED, DatabaseSchema::MODIFIED, DatabaseSchema::DUE_DATE, DatabaseSchema::STATUS, DatabaseSchema::TITLE, DatabaseSchema::DESCRIPTION, DatabaseSchema::KEY_ITEM_ID));

    QString currentTime = DatabaseUtils::getCurrentDateTime();

    query.bindValue(QStringLiteral(":created"), currentTime);
    query.bindValue(QStringLiteral(":modified"), currentTime);
    query.bindValue(QStringLiteral(":dueDate"), task.getDueDate());
    query.bindValue(QStringLiteral(":status"), task.getStatus());
    query.bindValue(QStringLiteral(":title"), task.getTitle());
    query.bindValue(QStringLiteral(":description"), task.getDescription());
    if (task.getItemId() != 0)
    {
        query.bindValue(QStringLiteral(":itemId"), task.getItemId());
    }

    if (!query.exec())
    {
        qDebug() << "Error inserting record " << DatabaseSchema::TABLE_TASKS;
        qDebug() << query.lastQuery() << query.lastError();
        return false;
    }

    return true;
}

bool TaskDatabase::updateEntry(const Task &task) const
{
    QSqlQuery query;

    query.prepare(QStringLiteral(
                      "UPDATE %1 SET %2=:modified, %3=:dueDate, %4=:status, %5=:title, %6=:description, %7=:itemId "
                      "WHERE id=:taskId")
                      .arg(DatabaseSchema::TABLE_TASKS, DatabaseSchema::MODIFIED, DatabaseSchema::DUE_DATE, DatabaseSchema::STATUS, DatabaseSchema::TITLE, DatabaseSchema::DESCRIPTION, DatabaseSchema::KEY_ITEM_ID));

    QString currentTime = DatabaseUtils::getCurrentDateTime();

    query.bindValue(QStringLiteral(":modified"), currentTime);
    qDebug() << task.getDueDate();
    query.bindValue(QStringLiteral(":dueDate"), task.getDueDate());
    query.bindValue(QStringLiteral(":status"), task.getStatus());
    query.bindValue(QStringLiteral(":title"), task.getTitle());
    query.bindValue(QStringLiteral(":description"), task.getDescription());
    query.bindValue(QStringLiteral(":itemId"), task.getItemId());

    query.bindValue(QStringLiteral(":taskId"), task.getId());

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
