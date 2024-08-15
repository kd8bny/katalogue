#include "attributeDatabase.h"

AttributeDatabase::AttributeDatabase(QObject *parent)
    : QObject(parent)
{
}

bool AttributeDatabase::insertEntry(const Attribute *attribute) const
{
    QSqlQuery query;

    query.prepare(
        QStringLiteral("INSERT INTO %1 (%2, %3, %4, %5, %6, %7) "
                       "VALUES (:created, :modified, :key, :value, :category, :attributeId)")
            .arg(DatabaseSchema::TABLE_ATTRIBUTES, DatabaseSchema::CREATED, DatabaseSchema::MODIFIED,
                 DatabaseSchema::KEY, DatabaseSchema::VALUE, DatabaseSchema::CATEGORY, DatabaseSchema::KEY_ITEM_ID));

    QString currentTime = DatabaseUtils::getCurrentDateTime();

    query.bindValue(QStringLiteral(":created"), currentTime);
    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":key"), attribute->getKey());
    query.bindValue(QStringLiteral(":value"), attribute->getValue());
    query.bindValue(QStringLiteral(":category"), attribute->getCategory());
    query.bindValue(QStringLiteral(":attributeId"), attribute->getItemId());

    if (!query.exec())
    {

        qDebug() << "Error inserting record " << DatabaseSchema::TABLE_ATTRIBUTES;
        qDebug() << query.lastQuery() << query.lastError();
        return false;
    }

    return true;
}

bool AttributeDatabase::updateEntry(const Attribute *attribute) const
{
    QSqlQuery query;

    query.prepare(
        QStringLiteral("UPDATE %1 SET %2=:modified, %3=:key, %4=:value, %5=:category WHERE id=:attributeId")
            .arg(DatabaseSchema::TABLE_ATTRIBUTES, DatabaseSchema::MODIFIED, DatabaseSchema::KEY,
                 DatabaseSchema::VALUE, DatabaseSchema::CATEGORY));

    QString currentTime = DatabaseUtils::getCurrentDateTime();

    query.bindValue(QStringLiteral(":modified"), currentTime);

    query.bindValue(QStringLiteral(":key"), attribute->getKey());
    query.bindValue(QStringLiteral(":value"), attribute->getValue());
    query.bindValue(QStringLiteral(":category"), attribute->getCategory());
    query.bindValue(QStringLiteral(":attributeId"), attribute->getItemId());

    query.bindValue(QStringLiteral(":attributeId"), attribute->getId());

    if (!query.exec())
    {
        qDebug() << "Error updating record " << DatabaseSchema::TABLE_ATTRIBUTES;
        qDebug() << query.lastQuery() << query.lastError();
        return false;
    }
    return true;
}

bool AttributeDatabase::deleteEntryById(int attributeId) const
{
    QSqlQuery query;

    query.prepare(QStringLiteral("DELETE FROM %1 WHERE id=:attributeId")
                      .arg(DatabaseSchema::TABLE_ATTRIBUTES));

    query.bindValue(QStringLiteral(":attributeId"), attributeId);

    if (!query.exec())
    {
        qDebug() << "Error removing record" << DatabaseSchema::TABLE_ATTRIBUTES;
        qDebug() << query.lastQuery() << query.lastError();
        return false;
    }

    return true;
}

Attribute *AttributeDatabase::getEntryById(const int id) const
{
    QSqlQuery query;

    query.prepare(
        QStringLiteral("SELECT id, %1, %2, %3, %4 FROM %5 WHERE id=:id")
            .arg(DatabaseSchema::KEY, DatabaseSchema::VALUE, DatabaseSchema::CATEGORY,
                 DatabaseSchema::KEY_ITEM_ID, DatabaseSchema::TABLE_ATTRIBUTES));

    query.bindValue(QStringLiteral(":id"), id);

    if (!query.exec())
    {
        qDebug() << "Error getting attribute entry " << query.lastError();
        qDebug() << query.lastQuery() << query.lastError();
        return nullptr;
    }
    query.next();

    EntryFactory entryFactory;
    std::unique_ptr<Attribute> attribute = entryFactory.createAttribute();

    attribute->setId(id);
    attribute->setCreatedDate(query.value(1).toString());
    attribute->setModifiedDate(query.value(2).toString());
    attribute->setKey(query.value(3).toString());
    attribute->setValue(query.value(4).toString());
    attribute->setCategory(query.value(5).toString());
    attribute->setItemId(query.value(6).toInt());

    return attribute.release();
}

Attribute *AttributeDatabase::getNewEntry() const
{
    EntryFactory entryFactory;
    std::unique_ptr<Attribute> attribute = entryFactory.createAttribute();

    return attribute.release();
}
