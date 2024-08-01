#include <QtTest>

#include "data/database.h"

class DatabaseAttributeTest : public QObject
{
    Q_OBJECT
    const QString testDBPath = QStringLiteral("../tests/");
    const QString DATABASE_NAME = QStringLiteral("katalogue.db");

private Q_SLOTS:
    void insertAttributeEntry() const;
    void updateAttributeEntry() const;
    void deleteAttributeEntry() const;
};

/*
 * Database CRUD Tests
 */

void DatabaseAttributeTest::insertAttributeEntry() const
{
    /*Test Attribute Insert*/
    Database katalogue_db;
    bool DB_OPEN = katalogue_db.connectKatalogueDb(this->testDBPath);
    qDebug() << "db open" << DB_OPEN;

    QVERIFY(DB_OPEN == true);

    QList<QVariantList> attributes;

    // QVariantList key, value, category, parent
    // Normal
    attributes.append(QVariantList({QStringLiteral("key"), QStringLiteral("value"), QStringLiteral("category"), 1}));
    // Minimum input
    attributes.append(QVariantList({QStringLiteral(""), QStringLiteral(""), QStringLiteral(""), 1}));
    // qDebug() << attributes;

    // Inserts correctly
    for (int i = 0; i < attributes.length(); i++)
    {
        QVariantList AttributeAsList;
        AttributeAsList = attributes.value(i);

        Attribute attribute(-1);
        attribute.setKey(AttributeAsList.value(0).toString());
        attribute.setValue(AttributeAsList.value(1).toString());
        attribute.setCategory(AttributeAsList.value(2).toString());
        attribute.setItemId(AttributeAsList.value(3).toInt());

        QVERIFY(katalogue_db.insertAttributeEntry(attribute) == true);
    }

    // Validate data
    const QString modelQueryBase = QStringLiteral("SELECT id, %1, %2, %3, %4 FROM %5 ")
                                       .arg(Database::KEY, Database::VALUE, Database::CATEGORY, Database::KEY_ITEM_ID,
                                            Database::TABLE_ATTRIBUTES);

    QSqlQuery query;
    query.exec(modelQueryBase);
    for (int i = 0; i < attributes.length(); i++)
    {
        int attributeId = i + 1;

        query.next();
        // qDebug() << query.record();

        QVERIFY(query.value(0).toInt() == attributeId);                     // id
        QVERIFY(query.value(1).toString() == attributes.value(i).value(0)); // Key
        QVERIFY(query.value(2).toString() == attributes.value(i).value(1)); // Value
        QVERIFY(query.value(3).toString() == attributes.value(i).value(2)); // Category
        QVERIFY(query.value(4).toInt() == attributes.value(i).value(3));    // parent
    }
}

void DatabaseAttributeTest::updateAttributeEntry() const
{
    /*Test Attribute Insert*/
    Database katalogue_db;
    bool DB_OPEN = katalogue_db.connectKatalogueDb(this->testDBPath);
    qDebug() << "db open" << DB_OPEN;
    QVERIFY(DB_OPEN == true);

    Attribute refAttribute(-1);
    refAttribute.setKey(QStringLiteral("refKey"));
    refAttribute.setValue(QStringLiteral("refValue"));
    refAttribute.setCategory(QStringLiteral("refCat"));
    refAttribute.setItemId(1);

    QVERIFY2(katalogue_db.insertAttributeEntry(refAttribute) == true, "Reference Attribute added");

    QVariantList attributeFields = {
        QStringLiteral("refKeyUpdated"),
        QStringLiteral("refValueUpdated"),
        QStringLiteral("refCatUpdated"),
        4,
    };

    // Query for reference attribute id 3
    const QString record3Query = QStringLiteral("SELECT %1, %2, %3, %4 FROM %5 WHERE id=3")
                                     .arg(Database::KEY, Database::VALUE, Database::CATEGORY,
                                          Database::KEY_ITEM_ID, Database::TABLE_ATTRIBUTES);
    QSqlQuery query;
    query.exec(record3Query);
    query.next();
    // qDebug() << query.record();

    // Build Attribute 3
    Attribute attribute3(3);
    attribute3.setKey(query.value(0).toString());
    attribute3.setValue(query.value(1).toString());
    attribute3.setCategory(query.value(2).toString());
    attribute3.setItemId(query.value(3).toInt());

    // Start Test
    // Update Key
    attribute3.setKey(attributeFields.value(0).toString());
    QVERIFY(katalogue_db.updateAttributeEntry(attribute3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(0).toString() == attributeFields.value(0).toString());

    // Update Value
    attribute3.setValue(attributeFields.value(1).toString());
    QVERIFY(katalogue_db.updateAttributeEntry(attribute3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(1).toString() == attributeFields.value(1).toString());

    // Update Category
    attribute3.setCategory(attributeFields.value(2).toString());
    QVERIFY(katalogue_db.updateAttributeEntry(attribute3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(2).toString() == attributeFields.value(2).toString());
}

void DatabaseAttributeTest::deleteAttributeEntry() const
{
    Database katalogue_db;
    // Get Data from Record 3
    // Check if KEY is null after delete
    const QString record3Query = QStringLiteral("SELECT %1 FROM %2 WHERE id=3")
                                     .arg(Database::KEY, Database::TABLE_ATTRIBUTES);

    QVERIFY2(katalogue_db.deleteAttributeEntry(3) == true, "Attribute 3 deleted");

    QSqlQuery query;
    query.exec(record3Query);
    query.next();
    QVERIFY(query.record().value(0) == QStringLiteral(""));
}

QTEST_MAIN(DatabaseAttributeTest)
#include "databaseAttributeTest.moc"
