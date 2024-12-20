#include "eventDatabase.h"

EventDatabase::EventDatabase(QObject *parent)
    : QObject(parent)
{
}

bool EventDatabase::insertEntry(const Event *event) const
{
    QSqlQuery query;

    query.prepare(
        QStringLiteral("INSERT INTO %1 ( %2, %3, %4, %5, %6, %7, %8, %9, %10, %11) "
                       "VALUES (:created, :modified, :date, :event, :servicer, :cost, :increment, :category, :comment, :itemId)")
            .arg(DatabaseSchema::TABLE_EVENTS, DatabaseSchema::CREATED, DatabaseSchema::MODIFIED, DatabaseSchema::DATE,
                 DatabaseSchema::EVENT, DatabaseSchema::SERVICER, DatabaseSchema::COST, DatabaseSchema::INCREMENT, DatabaseSchema::CATEGORY,
                 DatabaseSchema::COMMENT, DatabaseSchema::KEY_ITEM_ID));

    QString currentTime = DatabaseUtils::getCurrentDateTime();

    query.bindValue(QStringLiteral(":created"), currentTime);
    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":date"), event->getDate());
    query.bindValue(QStringLiteral(":event"), event->getEvent());
    query.bindValue(QStringLiteral(":servicer"), event->getServicer());
    query.bindValue(QStringLiteral(":cost"), event->getCost());
    query.bindValue(QStringLiteral(":increment"), event->getIncrement());
    query.bindValue(QStringLiteral(":category"), event->getCategory());
    query.bindValue(QStringLiteral(":comment"), event->getComment());
    query.bindValue(QStringLiteral(":itemId"), event->getItemId());

    if (!query.exec())
    {
        qDebug() << "Error inserting record " << DatabaseSchema::TABLE_EVENTS;
        qDebug() << query.lastQuery() << query.lastError();
        return false;
    }

    return true;
}

bool EventDatabase::updateEntry(const Event *event) const
{
    QSqlQuery query;

    query.prepare(
        QStringLiteral("UPDATE %1 SET %2=:modified, %3=:date, %4=:event, %5=:servicer, %6=:cost, %7=:increment, "
                       "%8=:category, %9=:comment WHERE id=:eventId")
            .arg(DatabaseSchema::TABLE_EVENTS, DatabaseSchema::MODIFIED, DatabaseSchema::DATE, DatabaseSchema::EVENT,
                 DatabaseSchema::SERVICER, DatabaseSchema::COST, DatabaseSchema::INCREMENT, DatabaseSchema::CATEGORY,
                 DatabaseSchema::COMMENT));

    QString currentTime = DatabaseUtils::getCurrentDateTime();

    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":date"), event->getDate());
    query.bindValue(QStringLiteral(":event"), event->getEvent());
    query.bindValue(QStringLiteral(":servicer"), event->getServicer());
    query.bindValue(QStringLiteral(":cost"), event->getCost());
    query.bindValue(QStringLiteral(":increment"), event->getIncrement());
    query.bindValue(QStringLiteral(":category"), event->getCategory());
    query.bindValue(QStringLiteral(":comment"), event->getComment());

    query.bindValue(QStringLiteral(":eventId"), event->getId());

    if (!query.exec())
    {
        qDebug() << "Error updating record " << DatabaseSchema::TABLE_EVENTS;
        qDebug() << query.lastQuery() << query.lastError();
        return false;
    }

    return true;
}

bool EventDatabase::deleteEntryById(const int id) const
{
    QSqlQuery query;

    query.prepare(
        QStringLiteral("DELETE FROM %1 WHERE id=:eventId").arg(DatabaseSchema::TABLE_EVENTS));
    query.bindValue(QStringLiteral(":eventId"), id);

    if (!query.exec())
    {
        qDebug() << "Error removing record " << DatabaseSchema::TABLE_EVENTS;
        qDebug() << query.lastQuery() << query.lastError();
        return false;
    }

    return true;
}

Event *EventDatabase::getEntryById(const int id) const
{
    EntryFactory entryFactory;
    Event *event = entryFactory.createEvent();

    if (id == 0)
    {
        return event;
    }

    QSqlQuery query;

    query.prepare(
        QStringLiteral("SELECT id, %1, %2, %3, %4, %5, %6, %7, %8, %9, %10 FROM %11 WHERE id=:id")
            .arg(DatabaseSchema::CREATED, DatabaseSchema::MODIFIED, DatabaseSchema::DATE, DatabaseSchema::EVENT,
                 DatabaseSchema::SERVICER, DatabaseSchema::COST, DatabaseSchema::INCREMENT, DatabaseSchema::CATEGORY,
                 DatabaseSchema::COMMENT, DatabaseSchema::KEY_ITEM_ID, DatabaseSchema::TABLE_EVENTS));

    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec())
    {
        qDebug() << "Error getting event entry " << query.lastError();
        qDebug() << query.lastQuery() << query.lastError();
        delete event;
        return nullptr;
    }
    query.next();

    event->setId(id);
    event->setCreatedDate(query.value(1).toString());
    event->setModifiedDate(query.value(2).toString());
    event->setDate(query.value(3).toString());
    event->setEvent(query.value(4).toString());
    event->setServicer(query.value(5).toString());
    event->setCost(query.value(6).toFloat());
    event->setIncrement(query.value(7).toFloat());
    event->setCategory(query.value(8).toString());
    event->setComment(query.value(9).toString());
    event->setItemId(query.value(10).toInt());

    return event;
}
