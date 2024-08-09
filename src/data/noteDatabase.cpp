#include "noteDatabase.h"

NoteDatabase::NoteDatabase(QObject *parent)
    : QObject(parent)
{
}

NoteDatabase::~NoteDatabase() = default;

bool NoteDatabase::insertEntry(const Note &note) const
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

    query.bindValue(QStringLiteral(":title"), note.getTitle());
    query.bindValue(QStringLiteral(":noteContent"), note.getNoteContent());
    if (note.getItemId() != 0)
    {
        query.bindValue(QStringLiteral(":itemId"), note.getItemId());
    }

    if (!query.exec())
    {
        qDebug() << "Error inserting record " << DatabaseSchema::TABLE_NOTES;
        qDebug() << query.lastQuery() << query.lastError();
        return false;
    }

    return true;
}

bool NoteDatabase::updateEntry(const Note &note) const
{
    QSqlQuery query;

    query.prepare(
        QStringLiteral("UPDATE %1 SET %2=:modified, %3=:title, %4=:noteContent, %5=:itemId WHERE id=:noteId")
            .arg(DatabaseSchema::TABLE_NOTES, DatabaseSchema::MODIFIED, DatabaseSchema::TITLE,
                 DatabaseSchema::NOTE_CONTENT, DatabaseSchema::KEY_ITEM_ID));

    QString currentTime = DatabaseUtils::getCurrentDateTime();

    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":title"), note.getTitle());
    query.bindValue(QStringLiteral(":noteContent"), note.getNoteContent());
    query.bindValue(QStringLiteral(":itemId"), note.getItemId());

    query.bindValue(QStringLiteral(":noteId"), note.getId());

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
