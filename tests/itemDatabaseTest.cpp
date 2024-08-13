#include <QtTest>
#include <QSqlRecord>

#include "databaseInit.h"
#include "databaseSchema.h"
#include "itemDatabase.h"

class ItemDatabaseTest : public QObject
{
    Q_OBJECT
    const QString testDBPath = QStringLiteral("../tests/");
    const QString DATABASE_NAME = QStringLiteral("katalogue.db");

private Q_SLOTS:
    void databaseConnected() const;
    void insertItemEntry() const;
    void insertItemComponentEntry() const;
    void updateEntry() const;
    void deleteItemEntry() const;
};

void ItemDatabaseTest::databaseConnected() const
{
    /*Clean up Database from previous Tests*/
    QFile file(this->testDBPath + this->DATABASE_NAME);
    qDebug() << "Database location" << this->testDBPath + this->DATABASE_NAME << "exists " << file.exists();
    if (file.exists())
    {
        file.remove();
        QVERIFY(file.exists() == false);
    }

    /*Test Database creation*/
    DatabaseInit init_db;
    bool DB_OPEN = init_db.connectKatalogueDb(this->testDBPath);
    QVERIFY2(DB_OPEN == true, "db open");

    /*Check Database Settings*/
    QSqlQuery query;
    query.exec(QStringLiteral("PRAGMA user_version"));
    query.next();
    QVERIFY(query.value(0).toInt() == 1);

    query.exec(QStringLiteral("PRAGMA foreign_keys"));
    query.next();
    QVERIFY(query.value(0).toInt() == 1);

    /*Remove Database File*/
    qDebug() << "Database location" << this->testDBPath + this->DATABASE_NAME << "exists " << file.exists();
    file.remove();
    QVERIFY(file.exists() == false);
}

/*
 * Database CRUD Tests
 */

void ItemDatabaseTest::insertItemEntry() const
{
    /*Test Item Insert*/
    DatabaseInit init_db;
    bool DB_OPEN = init_db.connectKatalogueDb(this->testDBPath);
    QVERIFY2(DB_OPEN == true, "db open");

    ItemDatabase katalogue_db;

    QList<QVariantList> items;

    // QVariantList({id, name, make, model, year, type, archived, user_order, parent}));
    // Normal
    items.append(QVariantList({QStringLiteral("testVehicleName"), QStringLiteral("testVehicleMake"),
                               QStringLiteral("testVehicleModel"), 2000, QStringLiteral("testVehicleType"),
                               false, -1, 0}));
    // Archived
    items.append(QVariantList({QStringLiteral("testVehicleArchivedName"), QStringLiteral("testVehicleArchivedMake"),
                               QStringLiteral("testVehicleArchivedModel"), 1000,
                               QStringLiteral("testVehicleArchivedType"), true, -1, 0}));
    // Minimum input
    items.append(QVariantList({QStringLiteral(""), QStringLiteral(""), QStringLiteral(""), 0, QStringLiteral(""),
                               false, -1, 0}));
    // qDebug() << items;

    // Inserts correctly
    for (int i = 0; i < items.length(); i++)
    {

        EntryFactory entryFactory;
        std::unique_ptr<Item> item_ptr = entryFactory.createItem();
        Item *item = item_ptr.release();

        QVariantList itemAsList;
        itemAsList = items.value(i);

        item->setName(itemAsList.value(0).toString());
        item->setMake(itemAsList.value(1).toString());
        item->setModel(itemAsList.value(2).toString());
        item->setYear(itemAsList.value(3).toInt());
        item->setType(itemAsList.value(4).toString());
        item->setArchived(itemAsList.value(5).toBool());
        // Ignore user order and set as null
        item->setItemId(itemAsList.value(7).toInt());

        QVERIFY2(katalogue_db.insertEntry(item) == true, "item inserted");

        delete item;
    }

    // Validate data
    const QString modelQueryBase = QStringLiteral(
                                       "SELECT id, %1, %2, %3, %4, %5, %6, %7, %8 FROM %9")
                                       .arg(DatabaseSchema::NAME, DatabaseSchema::MAKE, DatabaseSchema::MODEL, DatabaseSchema::YEAR,
                                            DatabaseSchema::TYPE, DatabaseSchema::ARCHIVED, DatabaseSchema::USER_ORDER,
                                            DatabaseSchema::KEY_ITEM_ID, DatabaseSchema::TABLE_ITEMS);
    QSqlQuery query;
    query.exec(modelQueryBase);
    for (int i = 0; i < items.length(); i++)
    {
        int itemId = i + 1;

        query.next();
        // qDebug() << query.record();

        QVERIFY(query.value(0).toInt() == itemId);                     // id
        QVERIFY(query.value(1).toString() == items.value(i).value(0)); // Name
        QVERIFY(query.value(2).toString() == items.value(i).value(1)); // Make
        QVERIFY(query.value(3).toString() == items.value(i).value(2)); // Model
        QVERIFY(query.value(4).toInt() == items.value(i).value(3));    // Year
        QVERIFY(query.value(5).toString() == items.value(i).value(4)); // Type
        QVERIFY(query.value(6).toBool() == items.value(i).value(5));   // Archived
        QVERIFY(query.value(7).isNull() == true);                      // User Order
        QVERIFY(query.value(8).isNull() == true);                      // parent
    }
}

void ItemDatabaseTest::insertItemComponentEntry() const
{
    /*Test Item Insert*/
    DatabaseInit init_db;
    bool DB_OPEN = init_db.connectKatalogueDb(this->testDBPath);
    QVERIFY2(DB_OPEN == true, "db open");

    ItemDatabase katalogue_db;

    QList<QVariantList> items;

    // QVariantList({id, name, make, model, year, type, archived, user_order, parent}));
    // Normal
    items.append(QVariantList({QStringLiteral("testComponentParent"), QStringLiteral("testComponentMake"),
                               QStringLiteral("testComponentModel"), 2000, QStringLiteral("testComponentType"),
                               false, -1, 0}));
    // Components
    items.append(QVariantList({QStringLiteral("testComponent1"), QStringLiteral("testComponent1Make"),
                               QStringLiteral("testComponent1Model"), 2022, QStringLiteral("testComponent1Type"),
                               false, -1, 4}));
    items.append(QVariantList({QStringLiteral("testComponent2"), QStringLiteral("testComponent2Make"),
                               QStringLiteral("testComponent2Model"), 2026, QStringLiteral("testComponent2Type"),
                               false, -1, 4}));

    // Inserts correctly
    for (int i = 0; i < items.length(); i++)
    {
        EntryFactory entryFactory;
        std::unique_ptr<Item> item = entryFactory.createItem();

        QVariantList itemAsList;
        itemAsList = items.value(i);

        item->setName(itemAsList.value(0).toString());
        item->setMake(itemAsList.value(1).toString());
        item->setModel(itemAsList.value(2).toString());
        item->setYear(itemAsList.value(3).toInt());
        item->setType(itemAsList.value(4).toString());
        item->setArchived(itemAsList.value(5).toBool());
        // Ignore user order and set as null
        item->setItemId(itemAsList.value(7).toInt());

        // qDebug() << "itemInserted: " << itemInserted;
        QVERIFY2(katalogue_db.insertEntry(item.release()) == true, "item component inserted");

        delete item.release();
    }

    // Validate data
    const QString modelQueryBase = QStringLiteral(
                                       "SELECT id, %1, %2, %3, %4, %5, %6, %7, %8 FROM %9")
                                       .arg(DatabaseSchema::NAME, DatabaseSchema::MAKE, DatabaseSchema::MODEL,
                                            DatabaseSchema::YEAR, DatabaseSchema::TYPE, DatabaseSchema::ARCHIVED,
                                            DatabaseSchema::USER_ORDER, DatabaseSchema::KEY_ITEM_ID,
                                            DatabaseSchema::TABLE_ITEMS);
    QSqlQuery query;
    query.exec(modelQueryBase);
    query.seek(2); // Seek to component item in sql results

    for (int i = 0; i < items.length(); i++)
    {
        int itemId = i + 4;
        query.next();
        // qDebug() << query.record();

        QVERIFY(query.value(0).toInt() == itemId);                     // id
        QVERIFY(query.value(1).toString() == items.value(i).value(0)); // Name
        QVERIFY(query.value(2).toString() == items.value(i).value(1)); // Make
        QVERIFY(query.value(3).toString() == items.value(i).value(2)); // Model
        QVERIFY(query.value(4).toInt() == items.value(i).value(3));    // Year
        QVERIFY(query.value(5).toString() == items.value(i).value(4)); // Type
        QVERIFY(query.value(6).toBool() == items.value(i).value(5));   // Archived
        QVERIFY(query.value(7).isNull() == true);                      // User Order
        if (itemId == 4)
        {
            QVERIFY(query.value(8).isNull() == true); // parent
        }
        else
        {
            QVERIFY(query.value(8).toInt() == 4); // parent
        }
    }
}

void ItemDatabaseTest::updateEntry() const
{
    /*Test Item Insert*/
    DatabaseInit init_db;
    bool DB_OPEN = init_db.connectKatalogueDb(this->testDBPath);
    QVERIFY2(DB_OPEN == true, "db open");

    ItemDatabase katalogue_db;

    QVariantList itemFields = {
        QStringLiteral("updatedName"), QStringLiteral("updatedMake"), QStringLiteral("updatedModel"), 2001,
        QStringLiteral("updatedType"), true, 2, 2};

    // Get Data from Record 3
    const QString record3Query = QStringLiteral(
                                     "SELECT %1, %2, %3, %4, %5, %6, %7, %8 FROM %9 WHERE id=3")
                                     .arg(DatabaseSchema::NAME, DatabaseSchema::MAKE, DatabaseSchema::MODEL, DatabaseSchema::YEAR,
                                          DatabaseSchema::TYPE, DatabaseSchema::ARCHIVED, DatabaseSchema::USER_ORDER,
                                          DatabaseSchema::KEY_ITEM_ID, DatabaseSchema::TABLE_ITEMS);
    QSqlQuery query;
    query.exec(record3Query);
    query.next();
    // qDebug() << query.record();

    EntryFactory entryFactory;
    std::unique_ptr<Item> item_ptr = entryFactory.createItem();

    Item *item3 = item_ptr.release();
    // Build item 3
    item3->setId(3);
    item3->setName(query.value(0).toString());
    item3->setMake(query.value(1).toString());
    item3->setModel(query.value(2).toString());
    item3->setYear(query.value(3).toInt());
    item3->setType(query.value(4).toString());
    item3->setArchived(query.value(5).toBool());
    // Ignore user order and set as null
    item3->setItemId(query.value(7).toInt());

    // Start Test
    // Update Name
    item3->setName(itemFields.value(0).toString());
    QVERIFY(katalogue_db.updateEntry(item3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(0).toString() == itemFields.value(0).toString());

    // Update Make
    item3->setMake(itemFields.value(1).toString());
    QVERIFY(katalogue_db.updateEntry(item3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(1).toString() == itemFields.value(1).toString());

    // Update Model
    item3->setModel(itemFields.value(2).toString());
    QVERIFY(katalogue_db.updateEntry(item3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(2).toString() == itemFields.value(2).toString());

    // Update Year
    item3->setYear(itemFields.value(3).toInt());
    QVERIFY(katalogue_db.updateEntry(item3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(3).toInt() == itemFields.value(3).toInt());

    // Update Type
    item3->setType(itemFields.value(4).toString());
    QVERIFY(katalogue_db.updateEntry(item3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(4).toString() == itemFields.value(4).toString());

    // Update Parent
    item3->setItemId(itemFields.value(7).toInt());
    QVERIFY(katalogue_db.updateEntry(item3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(7).toInt() == itemFields.value(7).toInt());
    // Update User Order
    // Uses updateItemUserOrder
    QVERIFY(katalogue_db.setUserOrder(3, itemFields.value(6).toInt()) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(6).toInt() == itemFields.value(6).toInt());

    // Update Archived
    // Uses updateItemArchived
    QVERIFY(katalogue_db.setArchived(3, itemFields.value(5).toBool()) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY2(query.value(5).toBool() == itemFields.value(5).toBool(), "Update Archived");
}

void ItemDatabaseTest::deleteItemEntry() const
{
    DatabaseInit init_db;
    bool DB_OPEN = init_db.connectKatalogueDb(this->testDBPath);
    QVERIFY2(DB_OPEN == true, "db open");

    ItemDatabase katalogue_db;

    // Get Data from Record 3
    const QString record3Query = QStringLiteral("SELECT %1 FROM %2 WHERE id=3")
                                     .arg(DatabaseSchema::NAME, DatabaseSchema::TABLE_ITEMS);

    QVERIFY2(katalogue_db.deleteEntryById(3) == true, "item 3 deleted");

    QSqlQuery query;
    query.exec(record3Query);
    query.next();
    QVERIFY(query.record().value(0) == QStringLiteral(""));
}

QTEST_MAIN(ItemDatabaseTest)

#include "itemDatabaseTest.moc"
