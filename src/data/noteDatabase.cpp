#include "noteDatabase.h"

NoteDatabase::NoteDatabase(QObject *parent)
    : QObject(parent)
{
}

bool NoteDatabase::insertEntry(const Note *note) const
{
    QSqlQuery query;

    query.prepare(
        QStringLiteral("INSERT INTO %1 (%2, %3, %4, %5, %6) "
                       "VALUES (:created, :modified, :title, :noteContent, :itemId)")
            .arg(DatabaseSchema::TABLE_NOTES, DatabaseSchema::CREATED, DatabaseSchema::MODIFIED,
                 DatabaseSchema::TITLE, DatabaseSchema::NOTE_CONTENT, DatabaseSchema::KEY_ITEM_ID));

    QString currentTime = DatabaseUtils::getCurrentDateTime();

    query.bindValue(QStringLiteral(":created"), currentTime);
    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":title"), note->getTitle());
    query.bindValue(QStringLiteral(":noteContent"), note->getNoteContent());
    if (note->getItemId() != 0)
    {
        query.bindValue(QStringLiteral(":itemId"), note->getItemId());
    }

    if (!query.exec())
    {
        qDebug() << "Error inserting record " << DatabaseSchema::TABLE_NOTES;
        qDebug() << query.lastQuery() << query.lastError();
        return false;
    }

    return true;
}

bool NoteDatabase::updateEntry(const Note *note) const
{
    QSqlQuery query;

    query.prepare(
        QStringLiteral("UPDATE %1 SET %2=:modified, %3=:title, %4=:noteContent, %5=:itemId WHERE id=:noteId")
            .arg(DatabaseSchema::TABLE_NOTES, DatabaseSchema::MODIFIED, DatabaseSchema::TITLE,
                 DatabaseSchema::NOTE_CONTENT, DatabaseSchema::KEY_ITEM_ID));

    QString currentTime = DatabaseUtils::getCurrentDateTime();

    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":title"), note->getTitle());
    query.bindValue(QStringLiteral(":noteContent"), note->getNoteContent());
    if (note->getItemId() != 0)
    {
        query.bindValue(QStringLiteral(":itemId"), note->getItemId());
    }

    query.bindValue(QStringLiteral(":noteId"), note->getId());

    if (!query.exec())
    {
        qDebug() << "Error updating record " << DatabaseSchema::TABLE_NOTES;
        qDebug() << query.lastQuery() << query.lastError();
        return false;
    }

    return true;
}

bool NoteDatabase::deleteEntryById(int id) const
{
    QSqlQuery query;

    query.prepare(QStringLiteral("DELETE FROM %1 WHERE id=:noteId").arg(DatabaseSchema::TABLE_NOTES));

    query.bindValue(QStringLiteral(":noteId"), id);

    if (!query.exec())
    {
        qDebug() << "Error deleting record " << DatabaseSchema::TABLE_NOTES;
        qDebug() << query.lastQuery() << query.lastError();
        return false;
    }

    return true;
}

Note *NoteDatabase::getEntryById(const int id) const
{
    EntryFactory entryFactory;
    Note *note = entryFactory.createNote();

    if (id == 0)
    {
        return note;
    }

    QSqlQuery query;

    query.prepare(
        QStringLiteral("SELECT id, %1, %2, %3, %4, %5 FROM %6 WHERE id=:id")
            .arg(DatabaseSchema::CREATED, DatabaseSchema::MODIFIED, DatabaseSchema::TITLE, DatabaseSchema::NOTE_CONTENT,
                 DatabaseSchema::KEY_ITEM_ID, DatabaseSchema::TABLE_NOTES));

    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec())
    {
        qDebug() << "Error getting note entry " << query.lastError();
        qDebug() << query.lastQuery() << query.lastError();
        return nullptr;
    }
    query.next();

    note->setId(id);
    note->setCreatedDate(query.value(1).toString());
    note->setModifiedDate(query.value(2).toString());
    note->setTitle(query.value(3).toString());
    note->setNoteContent(query.value(4).toString());
    note->setItemId(query.value(5).toInt());

    return note;
}
