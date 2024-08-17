#include <QtTest>
#include <QSqlRecord>

#include "databaseInit.h"
#include "databaseSchema.h"
#include "attributeDatabase.h"

class AttributeDatabaseTest : public QObject
{
    Q_OBJECT
    const QString testDBPath = QStringLiteral("../tests/");
    const QString DATABASE_NAME = QStringLiteral("katalogue.db");

private Q_SLOTS:
    void insertAttributeEntry() const;
    void updateEntry() const;
    void deleteAttributeEntry() const;
};

/*
 * Database CRUD Tests
 */

void AttributeDatabaseTest::insertAttributeEntry() const
{
    /*Test Attribute Insert*/
    DatabaseInit init_db;
    bool DB_OPEN = init_db.connectKatalogueDb(this->testDBPath);
    QVERIFY2(DB_OPEN == true, "db open");

    AttributeDatabase katalogue_db;

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

        EntryFactory entryFactory;
        Attribute *attribute = entryFactory.createAttribute();

        QVariantList AttributeAsList;
        AttributeAsList = attributes.value(i);

        attribute->setKey(AttributeAsList.value(0).toString());
        attribute->setValue(AttributeAsList.value(1).toString());
        attribute->setCategory(AttributeAsList.value(2).toString());
        attribute->setItemId(AttributeAsList.value(3).toInt());

        QVERIFY(katalogue_db.insertEntry(attribute) == true);

        delete attribute;
    }

    // Validate data
    const QString modelQueryBase = QStringLiteral("SELECT id, %1, %2, %3, %4 FROM %5 ")
                                       .arg(DatabaseSchema::KEY, DatabaseSchema::VALUE, DatabaseSchema::CATEGORY, DatabaseSchema::KEY_ITEM_ID,
                                            DatabaseSchema::TABLE_ATTRIBUTES);

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

void AttributeDatabaseTest::updateEntry() const
{
    /*Test Attribute Insert*/
    DatabaseInit init_db;
    bool DB_OPEN = init_db.connectKatalogueDb(this->testDBPath);
    QVERIFY2(DB_OPEN == true, "db open");

    AttributeDatabase katalogue_db;

    EntryFactory entryFactory;
    Attribute *refAttribute = entryFactory.createAttribute();

    refAttribute->setKey(QStringLiteral("refKey"));
    refAttribute->setValue(QStringLiteral("refValue"));
    refAttribute->setCategory(QStringLiteral("refCat"));
    refAttribute->setItemId(1);

    QVERIFY2(katalogue_db.insertEntry(refAttribute) == true, "Reference Attribute added");

    delete refAttribute;

    QVariantList attributeFields = {
        QStringLiteral("refKeyUpdated"),
        QStringLiteral("refValueUpdated"),
        QStringLiteral("refCatUpdated"),
        4,
    };

    // Query for reference attribute id 3
    const QString record3Query = QStringLiteral("SELECT %1, %2, %3, %4 FROM %5 WHERE id=3")
                                     .arg(DatabaseSchema::KEY, DatabaseSchema::VALUE, DatabaseSchema::CATEGORY,
                                          DatabaseSchema::KEY_ITEM_ID, DatabaseSchema::TABLE_ATTRIBUTES);
    QSqlQuery query;
    query.exec(record3Query);
    query.next();
    // qDebug() << query.record();

    // Build Attribute 3
    Attribute *attribute3 = entryFactory.createAttribute();
    attribute3->setId(3);
    attribute3->setKey(query.value(0).toString());
    attribute3->setValue(query.value(1).toString());
    attribute3->setCategory(query.value(2).toString());
    attribute3->setItemId(query.value(3).toInt());

    // Start Test
    // Update Key
    attribute3->setKey(attributeFields.value(0).toString());
    QVERIFY(katalogue_db.updateEntry(attribute3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(0).toString() == attributeFields.value(0).toString());

    // Update Value
    attribute3->setValue(attributeFields.value(1).toString());
    QVERIFY(katalogue_db.updateEntry(attribute3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(1).toString() == attributeFields.value(1).toString());

    // Update Category
    attribute3->setCategory(attributeFields.value(2).toString());
    QVERIFY(katalogue_db.updateEntry(attribute3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(2).toString() == attributeFields.value(2).toString());

    delete attribute3;
}

void AttributeDatabaseTest::deleteAttributeEntry() const
{
    AttributeDatabase katalogue_db;
    // Get Data from Record 3
    // Check if KEY is null after delete
    const QString record3Query = QStringLiteral("SELECT %1 FROM %2 WHERE id=3")
                                     .arg(DatabaseSchema::KEY, DatabaseSchema::TABLE_ATTRIBUTES);

    QVERIFY2(katalogue_db.deleteEntryById(3) == true, "Attribute 3 deleted");

    QSqlQuery query;
    query.exec(record3Query);
    query.next();
    QVERIFY(query.record().value(0) == QStringLiteral(""));
}

QTEST_MAIN(AttributeDatabaseTest)
#include "attributeDatabaseTest.moc"
