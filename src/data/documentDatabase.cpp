#include "documentDatabase.h"

#include <QString>
#include <QStringBuilder>

DocumentDatabase::DocumentDatabase(QObject *parent)
    : QObject(parent)
{
}

int DocumentDatabase::insertEntry(const Document *document) const
{
    QSqlQuery query;

    query.prepare(
        QStringLiteral("INSERT INTO %1 (%2, %3, %4, %5, %6, %7, %8, %9) "
                       "VALUES (:created, :modified, :name, :fileName, :data, :itemId, :eventId, :noteId)")
            .arg(DatabaseSchema::TABLE_DOCUMENTS, DatabaseSchema::CREATED, DatabaseSchema::MODIFIED,
                 DatabaseSchema::NAME, DatabaseSchema::FILE_NAME, DatabaseSchema::DATA,
                 DatabaseSchema::KEY_ITEM_ID, DatabaseSchema::KEY_EVENT_ID, DatabaseSchema::KEY_NOTE_ID));

    QString currentTime = DatabaseUtils::getCurrentDateTime();

    query.bindValue(QStringLiteral(":created"), currentTime);
    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":name"), document->getName());
    query.bindValue(QStringLiteral(":fileName"), document->getFileName());
    query.bindValue(QStringLiteral(":data"), document->getDataLazy());
    if (document->getItemId() != 0)
    {
        query.bindValue(QStringLiteral(":itemId"), document->getItemId());
    }
    if (document->getEventId() != 0)
    {
        query.bindValue(QStringLiteral(":eventId"), document->getEventId());
    }
    if (document->getNoteId() != 0)
    {
        query.bindValue(QStringLiteral(":noteId"), document->getNoteId());
    }

    if (!query.exec())
    {
        qDebug() << "Error inserting record " << DatabaseSchema::TABLE_DOCUMENTS;
        qDebug() << query.lastQuery() << query.lastError();
        return -1;
    }

    auto id = query.lastInsertId().toInt();

    return id;
}

bool DocumentDatabase::updateEntry(const Document *document) const
{
    QSqlQuery query;

    query.prepare(
        QStringLiteral("UPDATE %1 SET %2=:modified, %3=:name, %4=:fileName, %5=:data, "
                       "%6 =:itemId, %7 =:eventId, %8=:noteId WHERE id=:documentId")
            .arg(DatabaseSchema::TABLE_DOCUMENTS, DatabaseSchema::MODIFIED, DatabaseSchema::NAME,
                 DatabaseSchema::FILE_NAME, DatabaseSchema::DATA,
                 DatabaseSchema::KEY_ITEM_ID, DatabaseSchema::KEY_EVENT_ID, DatabaseSchema::KEY_NOTE_ID));

    QString currentTime = DatabaseUtils::getCurrentDateTime();

    query.bindValue(QStringLiteral(":created"), currentTime);
    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":name"), document->getName());
    query.bindValue(QStringLiteral(":fileName"), document->getFileName());
    query.bindValue(QStringLiteral(":data"), document->getDataLazy());
    if (document->getItemId() != 0)
    {
        query.bindValue(QStringLiteral(":itemId"), document->getItemId());
    }
    if (document->getEventId() != 0)
    {
        query.bindValue(QStringLiteral(":eventId"), document->getEventId());
    }
    if (document->getNoteId() != 0)
    {
        query.bindValue(QStringLiteral(":noteId"), document->getNoteId());
    }

    if (!query.exec())
    {
        qDebug() << "Error updating record " << DatabaseSchema::TABLE_DOCUMENTS;
        qDebug() << query.lastQuery() << query.lastError();
        return false;
    }

    return true;
}

bool DocumentDatabase::deleteEntryById(int id) const
{
    QSqlQuery query;

    query.prepare(QStringLiteral("DELETE FROM %1 WHERE id=:documentId").arg(DatabaseSchema::TABLE_DOCUMENTS));

    query.bindValue(QStringLiteral(":documentId"), id);

    if (!query.exec())
    {
        qDebug() << "Error deleting record " << DatabaseSchema::TABLE_DOCUMENTS;
        qDebug() << query.lastQuery() << query.lastError();
        return false;
    }

    return true;
}

Document *DocumentDatabase::getEntryById(const int id) const
{
    EntryFactory entryFactory;
    Document *document = entryFactory.createDocument();

    if (id == 0)
    {
        return document;
    }

    QSqlQuery query;

    query.prepare(
        QStringLiteral("SELECT id, %1, %2, %3, %4, %5, %6, %7, %8 FROM %9 WHERE id=:id")
            .arg(DatabaseSchema::CREATED, DatabaseSchema::MODIFIED, DatabaseSchema::NAME, DatabaseSchema::FILE_NAME,
                 DatabaseSchema::DATA,
                 DatabaseSchema::KEY_ITEM_ID, DatabaseSchema::KEY_EVENT_ID, DatabaseSchema::KEY_NOTE_ID,
                 DatabaseSchema::TABLE_DOCUMENTS));

    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec())
    {
        qDebug() << "Error getting document entry " << query.lastError();
        qDebug() << query.lastQuery() << query.lastError();
        delete document;
        return nullptr;
    }
    query.next();

    document->setId(id);
    document->setCreatedDate(query.value(1).toString());
    document->setModifiedDate(query.value(2).toString());
    document->setName(query.value(3).toString());
    document->setFileName(query.value(4).toString());
    document->setDataLazy(query.value(5).toByteArray());
    document->setItemId(query.value(6).toInt());
    document->setEventId(query.value(7).toInt());
    document->setNoteId(query.value(8).toInt());

    return document;
}
