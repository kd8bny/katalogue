#include "itemDatabase.h"

ItemDatabase::ItemDatabase(QObject *parent)
    : QObject(parent)
{
}

bool ItemDatabase::insertEntry(const Item &item) const
{
    /*
     * Insert Item.
     * Components will have a Null foreign key
     */
    QSqlQuery query;

    query.prepare(
        QStringLiteral("INSERT INTO %1 (%2, %3, %4, %5, %6, %7, %8, %9, %10) "
                       "VALUES (:created, :modified, :name, :make, :model, :year, :type, :archived, :parent)")
            .arg(DatabaseSchema::TABLE_ITEMS, DatabaseSchema::CREATED, DatabaseSchema::MODIFIED,
                 DatabaseSchema::NAME, DatabaseSchema::MAKE, DatabaseSchema::MODEL, DatabaseSchema::YEAR,
                 DatabaseSchema::TYPE, DatabaseSchema::ARCHIVED, DatabaseSchema::KEY_ITEM_ID));

    QString currentTime = DatabaseUtils::getCurrentDateTime();

    query.bindValue(QStringLiteral(":created"), currentTime);
    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":name"), item.getName());
    query.bindValue(QStringLiteral(":make"), item.getMake());
    query.bindValue(QStringLiteral(":model"), item.getModel());
    query.bindValue(QStringLiteral(":year"), item.getYear());
    query.bindValue(QStringLiteral(":type"), item.getType());
    query.bindValue(QStringLiteral(":archived"), item.getArchived());
    if (item.getItemId() != 0)
    {
        query.bindValue(QStringLiteral(":parent"), item.getItemId());
    }

    if (!query.exec())
    {
        qDebug() << "Error inserting record " << query.lastError();
        return false;
    }

    return true;
}

bool ItemDatabase::updateEntry(const Item &item) const
{
    /*
     * Update Item.
     */
    QSqlQuery query;

    query.prepare(
        QStringLiteral("UPDATE %1 SET %2=:modified, %3=:name, %4=:make, %5=:model, %6=:year, %7=:type, %8=:parent "
                       "WHERE id=:id")
            .arg(DatabaseSchema::TABLE_ITEMS, DatabaseSchema::MODIFIED, DatabaseSchema::NAME, DatabaseSchema::MAKE,
                 DatabaseSchema::MODEL, DatabaseSchema::YEAR, DatabaseSchema::TYPE, DatabaseSchema::KEY_ITEM_ID));

    QString currentTime = DatabaseUtils::getCurrentDateTime();

    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":id"), item.getId());
    query.bindValue(QStringLiteral(":name"), item.getName());
    query.bindValue(QStringLiteral(":make"), item.getMake());
    query.bindValue(QStringLiteral(":model"), item.getModel());
    query.bindValue(QStringLiteral(":year"), item.getYear());
    query.bindValue(QStringLiteral(":type"), item.getType());
    if (item.getItemId() != 0)
    {
        query.bindValue(QStringLiteral(":parent"), item.getItemId());
    }

    if (!query.exec())
    {
        qDebug() << "Error updating record " << query.lastError();
        qDebug() << query.lastQuery();
        return false;
    }

    return true;
}

bool ItemDatabase::deleteEntryById(const int id) const
{
    /*
     * Delete Item.
     */
    QSqlQuery query;

    query.prepare(
        QStringLiteral("DELETE FROM %1 WHERE id=:itemId").arg(DatabaseSchema::TABLE_ITEMS));
    qDebug() << QStringLiteral("DELETE FROM %1 WHERE id=:itemId").arg(DatabaseSchema::TABLE_ITEMS);

    query.bindValue(QStringLiteral(":itemId"), id);

    if (!query.exec())
    {
        qDebug() << "Error removing record " << query.lastError();
        return false;
    }

    return true;
}

bool ItemDatabase::setArchived(const int id, const bool archived) const
{
    QSqlQuery query;

    query.prepare(
        QStringLiteral("UPDATE %1 SET %2=:modified, %3=:archived WHERE id=:id")
            .arg(DatabaseSchema::TABLE_ITEMS, DatabaseSchema::MODIFIED, DatabaseSchema::ARCHIVED));

    QString currentTime = DatabaseUtils::getCurrentDateTime();

    query.bindValue(QStringLiteral(":modified"), currentTime);
    query.bindValue(QStringLiteral(":archived"), archived);
    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec())
    {
        qDebug() << "Error updating record archived " << query.lastError();
        qDebug() << query.lastQuery() << query.lastError();
        return false;
    }
    qDebug() << query.lastQuery() << query.lastError();

    return true;
}

bool ItemDatabase::setUserOrder(const int id, const int user_order) const
{
    QSqlQuery query;

    query.prepare(
        QStringLiteral("UPDATE %1 SET %2=:modified, %3=:user_order WHERE id=:id")
            .arg(DatabaseSchema::TABLE_ITEMS, DatabaseSchema::MODIFIED, DatabaseSchema::USER_ORDER));

    query.bindValue(QStringLiteral(":modified"), DatabaseUtils::getCurrentDateTime());
    query.bindValue(QStringLiteral(":user_order"), user_order);
    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec())
    {
        qDebug() << "Error updating record order " << query.lastError();
        qDebug() << query.lastQuery() << query.lastError();
        return false;
    }

    return true;
}
