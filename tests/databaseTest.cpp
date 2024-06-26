#include <QtTest>
#include "../src/data/database.h"

class DatabaseTest : public QObject
{
    Q_OBJECT
    const QString testDBPath = QStringLiteral("./tests/");
    const QString DATABASE_NAME = QStringLiteral("katalogue.db");

private Q_SLOTS:
    void databaseConnected() const;
    void insertItemEntry() const;
    void insertItemComponentEntry() const;
    void updateItemEntry() const;
    void databasePurged() const;
};

void DatabaseTest::databaseConnected() const
{
    /*Test Database creation*/
    Database katalogue_db;
    bool DB_OPEN = katalogue_db.connect(this->testDBPath);

    QVERIFY(DB_OPEN == true);

    /*Check Database Settings*/
    QSqlQuery query;
    query.exec(QStringLiteral("PRAGMA user_version"));
    query.next();
    QVERIFY(query.value(0).toInt() == 1);

    query.exec(QStringLiteral("PRAGMA foreign_keys"));
    query.next();
    qDebug() << query.record();
    // qDebug() << "here" << query.value(0);
    QVERIFY(query.value(0).toInt() == 1);

    QFile file(this->testDBPath + this->DATABASE_NAME);
    qDebug() << "Database location" << this->testDBPath + this->DATABASE_NAME << "exists " << file.exists();
    file.remove();
    QVERIFY(file.exists() == false);
}

void DatabaseTest::insertItemEntry() const
{
    /*Test Item Insert*/
    Database katalogue_db;
    bool DB_OPEN = katalogue_db.connect(this->testDBPath);
    qDebug() << "db open" << DB_OPEN;

    QVERIFY(DB_OPEN == true);

    QList<QVariantList> items;

    // QVariantList({id, name, make, model, year, type, archived, user_order, parent}));
    // Normal
    items.append(QVariantList({QStringLiteral("testVehicleName"), QStringLiteral("testVehicleMake"),
                               QStringLiteral("testVehicleModel"), 2000, QStringLiteral("testVehicleType"),
                               false, -1, -1}));
    // Archived
    items.append(QVariantList({QStringLiteral("testVehicleArchivedName"), QStringLiteral("testVehicleArchivedMake"),
                               QStringLiteral("testVehicleArchivedModel"), 1000,
                               QStringLiteral("testVehicleArchivedType"), true, -1, -1}));
    // Minimum input
    items.append(QVariantList({QStringLiteral(""), QStringLiteral(""), QStringLiteral(""), 0, QStringLiteral(""),
                               false, -1, -1}));
    // qDebug() << items;

    // Inserts correctly
    for (int i = 0; i < items.length(); i++)
    {
        bool itemInserted;
        QVariantList itemAsList;
        itemAsList = items.value(i);

        Item item(-1); // (itemAsList.value(0).toInt())
        item.setName(itemAsList.value(0).toString());
        item.setMake(itemAsList.value(1).toString());
        item.setModel(itemAsList.value(2).toString());
        item.setYear(itemAsList.value(3).toInt());
        item.setType(itemAsList.value(4).toString());
        item.setArchived(itemAsList.value(5).toBool());
        // Ignore user order and set as null
        item.setParent(itemAsList.value(7).toInt());

        itemInserted = katalogue_db.insertItemEntry(item);
        // qDebug() << "itemInserted: " << itemInserted;
        QVERIFY(itemInserted == true);
    }

    // Validate data
    const QString modelQueryBase = QStringLiteral(
                                       "SELECT id, %1, %2, %3, %4, %5, %6, %7, %8 FROM %9")
                                       .arg(Database::NAME, Database::MAKE, Database::MODEL, Database::YEAR,
                                            Database::TYPE, Database::ARCHIVED, Database::USER_ORDER,
                                            Database::KEY_ITEM_ID, Database::TABLE_ITEMS);
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

void DatabaseTest::insertItemComponentEntry() const
{
    /*Test Item Insert*/
    Database katalogue_db;
    bool DB_OPEN = katalogue_db.connect(this->testDBPath);
    qDebug() << "db open" << DB_OPEN;

    QVERIFY(DB_OPEN == true);

    QList<QVariantList> items;

    // QVariantList({id, name, make, model, year, type, archived, user_order, parent}));
    // Normal
    items.append(QVariantList({QStringLiteral("testComponentParent"), QStringLiteral("testComponentMake"),
                               QStringLiteral("testComponentModel"), 2000, QStringLiteral("testComponentType"),
                               false, -1, -1}));
    // Components
    items.append(QVariantList({QStringLiteral("testComponent1"), QStringLiteral("testComponent1Make"),
                               QStringLiteral("testComponent1Model"), 2022, QStringLiteral("testComponent1Type"),
                               false, -1, 4}));
    items.append(QVariantList({QStringLiteral("testComponent2"), QStringLiteral("testComponent2Make"),
                               QStringLiteral("testComponent2Model"), 2026, QStringLiteral("testComponent2Type"),
                               false, -1, 4}));
    // TODO Archived Component
    //  items.append(QVariantList({QStringLiteral("testComponent2"), QStringLiteral("testComponent2Make"),
    //                             QStringLiteral("testComponent2Model"), 2026, QStringLiteral("testComponent2Type"),
    //                             true, -1, 4}));
    //  qDebug() << items;

    // Inserts correctly
    for (int i = 0; i < items.length(); i++)
    {
        bool itemInserted;
        QVariantList itemAsList;
        itemAsList = items.value(i);

        Item item(-1); // (itemAsList.value(0).toInt())
        item.setName(itemAsList.value(0).toString());
        item.setMake(itemAsList.value(1).toString());
        item.setModel(itemAsList.value(2).toString());
        item.setYear(itemAsList.value(3).toInt());
        item.setType(itemAsList.value(4).toString());
        item.setArchived(itemAsList.value(5).toBool());
        // Ignore user order and set as null
        item.setParent(itemAsList.value(7).toInt());

        itemInserted = katalogue_db.insertItemEntry(item);
        // qDebug() << "itemInserted: " << itemInserted;
        QVERIFY(itemInserted == true);
    }

    // Validate data
    const QString modelQueryBase = QStringLiteral(
                                       "SELECT id, %1, %2, %3, %4, %5, %6, %7, %8 FROM %9")
                                       .arg(Database::NAME, Database::MAKE, Database::MODEL, Database::YEAR,
                                            Database::TYPE, Database::ARCHIVED, Database::USER_ORDER,
                                            Database::KEY_ITEM_ID, Database::TABLE_ITEMS);
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

// TODO check model queries component,archive, parents... etc

// void DatabaseTest::updateItemEntry() const
// {
// }

// TODO addedit tests

// // Test notes not component
// this->notes.append(QVariantList({QStringLiteral("nox notes test"), QStringLiteral("stuff"), 2}));
// // Test notes without parent
// this->notes.append(QVariantList({QStringLiteral("Orphan note"), QStringLiteral("but thats okay?"), -1}));

// // Test notes without parent
// this->tasks.append(QVariantList({QStringLiteral("2024-09-19T00:00:00.000-05:00"), QStringLiteral("In Progress"), QStringLiteral("testtask"), QStringLiteral("deets"), 2}));
// // Test notes without parent
// this->tasks.append(QVariantList({QStringLiteral("2024-09-19T00:00:00.000-05:00"), QStringLiteral("In Progress"), QStringLiteral("thetask"), QStringLiteral("deets"), -1}));

void DatabaseTest::databasePurged() const
{
    /*Clean up Database from Tests*/
    QFile file(this->testDBPath + this->DATABASE_NAME);
    qDebug() << "Database location" << this->testDBPath + this->DATABASE_NAME << "exists " << file.exists();
    file.remove();
    QVERIFY(file.exists() == false);
}

QTEST_MAIN(DatabaseTest)
#include "databaseTest.moc"
