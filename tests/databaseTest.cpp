// #include <catch2/catch_test_macros.hpp>

#include "../src/data/database.h"
#include <QtTest>

class DatabaseTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void databaseConnected();
    void insertItemEntry();
};

void DatabaseTest::databaseConnected()
{
    /*Test Database creation*/
    Database katalogue_db;
    bool DB_OPEN = katalogue_db.connect(QStringLiteral("."));
    qDebug() << "db open" << DB_OPEN;

    QVERIFY(DB_OPEN == true);
    // TODO check settings
}

void DatabaseTest::insertItemEntry()
{
    /*Test Item Insert*/
    Database katalogue_db;
    bool DB_OPEN = katalogue_db.connect(QStringLiteral("."));
    qDebug() << "db open" << DB_OPEN;

    QVERIFY(DB_OPEN == true);

    QList<QVariantList> items;

    // QVariantList({id, name, make, model, year, type, archived, user_order, parent}));
    // Normal
    items.append(QVariantList({QStringLiteral("testVehicleName"), QStringLiteral("testVehicleMake"), QStringLiteral("testVehicleModel"), 2000, QStringLiteral("testVehicleType"), false, -1, -1}));
    // Archived
    items.append(QVariantList({QStringLiteral("testVehicleArchivedName"), QStringLiteral("testVehicleArchivedMake"), QStringLiteral("testVehicleArchivedModel"), 1000, QStringLiteral("testVehicleArchivedType"), true, -1, -1}));
    // Minimum input
    items.append(QVariantList({QStringLiteral("testMinInput"), QStringLiteral(""), QStringLiteral(""), 0, QStringLiteral(""), false, -1, -1}));
    // Component
    // items.append(QVariantList({QStringLiteral("testComponentParentName"), QStringLiteral(""), QStringLiteral(""), 1000, QStringLiteral("testVehicleArchivedType"), true, -1, -1}));
    // items.append(QVariantList({QStringLiteral("testComponentName"), QStringLiteral(""), QStringLiteral(""), 1000, QStringLiteral("testVehicleArchivedType"), true, -1, -1}));

    qDebug() << items;

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
        QVERIFY(itemInserted == true);
    }
}

QTEST_MAIN(DatabaseTest)
#include "databaseTest.moc"
