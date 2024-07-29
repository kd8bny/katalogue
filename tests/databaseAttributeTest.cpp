#include <QtTest>

#include "data/database.h"

class DatabaseAttributeTest : public QObject
{
    Q_OBJECT
    const QString testDBPath = QStringLiteral("../tests/");
    const QString DATABASE_NAME = QStringLiteral("katalogue.db");

private Q_SLOTS:
    void insertAttributeEntry() const;
    // void updateAttributeEntry() const;
    // void deleteAttributeEntry() const;
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
                                       .arg(Database::KEY, Database::VALUE, Database::CATEGORY, Database::KEY_ITEM_ID, Database::TABLE_ATTRIBUTES);

    QSqlQuery query;
    query.exec(modelQueryBase);
    for (int i = 0; i < attributes.length(); i++)
    {
        int attributeId = i + 1;

        query.next();
        qDebug() << query.record();

        QVERIFY(query.value(0).toInt() == attributeId);                     // id
        QVERIFY(query.value(1).toString() == attributes.value(i).value(0)); // Key
        QVERIFY(query.value(2).toString() == attributes.value(i).value(1)); // Value
        QVERIFY(query.value(3).toString() == attributes.value(i).value(2)); // Category
        QVERIFY(query.value(4).toInt() == 1);                               // parent
    }
}

// void DatabaseAttributeTest::insertAttributeComponentEntry() const
// {
//     /*Test Attribute Insert*/
//     Database katalogue_db;
//     bool DB_OPEN = katalogue_db.connectKatalogueDb(this->testDBPath);
//     QVERIFY2(DB_OPEN == true, "db open: " + DB_OPEN);

//     QList<QVariantList> Attributes;

//     // QVariantList({id, name, make, model, year, type, archived, user_order, parent}));
//     // Normal
//     Attributes.append(QVariantList({QStringLiteral("testComponentParent"), QStringLiteral("testComponentMake"),
//                                QStringLiteral("testComponentModel"), 2000, QStringLiteral("testComponentType"),
//                                false, -1, 0}));
//     // Components
//     Attributes.append(QVariantList({QStringLiteral("testComponent1"), QStringLiteral("testComponent1Make"),
//                                QStringLiteral("testComponent1Model"), 2022, QStringLiteral("testComponent1Type"),
//                                false, -1, 4}));
//     Attributes.append(QVariantList({QStringLiteral("testComponent2"), QStringLiteral("testComponent2Make"),
//                                QStringLiteral("testComponent2Model"), 2026, QStringLiteral("testComponent2Type"),
//                                false, -1, 4}));

//     // Inserts correctly
//     for (int i = 0; i < Attributes.length(); i++)
//     {
//         QVariantList AttributeAsList;
//         AttributeAsList = Attributes.value(i);

//         Attribute Attribute(-1); // (AttributeAsList.value(0).toInt())
//         Attribute.setName(AttributeAsList.value(0).toString());
//         Attribute.setMake(AttributeAsList.value(1).toString());
//         Attribute.setModel(AttributeAsList.value(2).toString());
//         Attribute.setYear(AttributeAsList.value(3).toInt());
//         Attribute.setType(AttributeAsList.value(4).toString());
//         Attribute.setArchived(AttributeAsList.value(5).toBool());
//         // Ignore user order and set as null
//         Attribute.setParent(AttributeAsList.value(7).toInt());

//         // qDebug() << "AttributeInserted: " << AttributeInserted;
//         QVERIFY2(katalogue_db.insertAttributeEntry(Attribute) == true, "Attribute inserted");
//     }

//     // Validate data
//     const QString modelQueryBase = QStringLiteral(
//                                        "SELECT id, %1, %2, %3, %4, %5, %6, %7, %8 FROM %9")
//                                        .arg(Database::NAME, Database::MAKE, Database::MODEL, Database::YEAR,
//                                             Database::TYPE, Database::ARCHIVED, Database::USER_ORDER,
//                                             Database::KEY_Attribute_ID, Database::TABLE_AttributeS);
//     QSqlQuery query;
//     query.exec(modelQueryBase);
//     query.seek(2); // Seek to component Attribute in sql results

//     for (int i = 0; i < Attributes.length(); i++)
//     {
//         int AttributeId = i + 4;
//         query.next();
//         // qDebug() << query.record();

//         QVERIFY(query.value(0).toInt() == AttributeId);                     // id
//         QVERIFY(query.value(1).toString() == Attributes.value(i).value(0)); // Name
//         QVERIFY(query.value(2).toString() == Attributes.value(i).value(1)); // Make
//         QVERIFY(query.value(3).toString() == Attributes.value(i).value(2)); // Model
//         QVERIFY(query.value(4).toInt() == Attributes.value(i).value(3));    // Year
//         QVERIFY(query.value(5).toString() == Attributes.value(i).value(4)); // Type
//         QVERIFY(query.value(6).toBool() == Attributes.value(i).value(5));   // Archived
//         QVERIFY(query.value(7).isNull() == true);                      // User Order
//         if (AttributeId == 4)
//         {
//             QVERIFY(query.value(8).isNull() == true); // parent
//         }
//         else
//         {
//             QVERIFY(query.value(8).toInt() == 4); // parent
//         }
//     }
// }

// void DatabaseAttributeTest::updateAttributeEntry() const
// {
//     /*Test Attribute Insert*/
//     Database katalogue_db;
//     bool DB_OPEN = katalogue_db.connectKatalogueDb(this->testDBPath);
//     qDebug() << "db open" << DB_OPEN;
//     QVERIFY(DB_OPEN == true);

//     QVariantList AttributeFields = {
//         QStringLiteral("updatedName"), QStringLiteral("updatedMake"), QStringLiteral("updatedModel"), 2001,
//         QStringLiteral("updatedType"), true, 2, 2};

//     // Get Data from Record 3
//     const QString record3Query = QStringLiteral(
//                                      "SELECT %1, %2, %3, %4, %5, %6, %7, %8 FROM %9 WHERE id=3")
//                                      .arg(Database::NAME, Database::MAKE, Database::MODEL, Database::YEAR,
//                                           Database::TYPE, Database::ARCHIVED, Database::USER_ORDER,
//                                           Database::KEY_Attribute_ID, Database::TABLE_AttributeS);
//     QSqlQuery query;
//     query.exec(record3Query);
//     query.next();
//     // qDebug() << query.record();

//     // Build Attribute 3
//     Attribute Attribute3(3);
//     Attribute3.setName(query.value(0).toString());
//     Attribute3.setMake(query.value(2).toString());
//     Attribute3.setModel(query.value(2).toString());
//     Attribute3.setYear(query.value(3).toInt());
//     Attribute3.setType(query.value(4).toString());
//     Attribute3.setArchived(query.value(5).toBool());
//     // Ignore user order and set as null
//     Attribute3.setParent(query.value(7).toInt());

//     qDebug() << Attribute3.asList();

//     // Start Test
//     // Update Name
//     Attribute3.setName(AttributeFields.value(0).toString());
//     QVERIFY(katalogue_db.updateAttributeEntry(Attribute3) == true);

//     query.exec(record3Query);
//     query.next();
//     QVERIFY(query.value(0).toString() == AttributeFields.value(0).toString());

//     // Update Make
//     Attribute3.setMake(AttributeFields.value(1).toString());
//     QVERIFY(katalogue_db.updateAttributeEntry(Attribute3) == true);

//     query.exec(record3Query);
//     query.next();
//     QVERIFY(query.value(1).toString() == AttributeFields.value(1).toString());

//     // Update Model
//     Attribute3.setModel(AttributeFields.value(2).toString());
//     QVERIFY(katalogue_db.updateAttributeEntry(Attribute3) == true);

//     query.exec(record3Query);
//     query.next();
//     QVERIFY(query.value(2).toString() == AttributeFields.value(2).toString());

//     // Update Year
//     Attribute3.setYear(AttributeFields.value(3).toInt());
//     QVERIFY(katalogue_db.updateAttributeEntry(Attribute3) == true);

//     query.exec(record3Query);
//     query.next();
//     QVERIFY(query.value(3).toInt() == AttributeFields.value(3).toInt());

//     // Update Type
//     Attribute3.setType(AttributeFields.value(4).toString());
//     QVERIFY(katalogue_db.updateAttributeEntry(Attribute3) == true);

//     query.exec(record3Query);
//     query.next();
//     QVERIFY(query.value(4).toString() == AttributeFields.value(4).toString());

//     // Update Parent
//     Attribute3.setParent(AttributeFields.value(7).toInt());
//     QVERIFY(katalogue_db.updateAttributeEntry(Attribute3) == true);

//     query.exec(record3Query);
//     query.next();
//     QVERIFY(query.value(7).toInt() == AttributeFields.value(7).toInt());
//     // Update User Order
//     // Uses updateAttributeUserOrder
//     QVERIFY(katalogue_db.updateAttributeUserOrder(3, AttributeFields.value(6).toInt()) == true);

//     query.exec(record3Query);
//     query.next();
//     QVERIFY(query.value(6).toInt() == AttributeFields.value(6).toInt());

//     // Update Archived
//     // Uses updateAttributeArchived
//     QVERIFY(katalogue_db.updateAttributeArchived(3, AttributeFields.value(5).toBool()) == true);

//     query.exec(record3Query);
//     query.next();
//     QVERIFY2(query.value(5).toBool() == AttributeFields.value(5).toBool(), "Update Archived");
// }

// void DatabaseAttributeTest::deleteAttributeEntry() const
// {
//     Database katalogue_db;
//     // Get Data from Record 3
//     const QString record3Query = QStringLiteral("SELECT %1 FROM %2 WHERE id=3")
//                                      .arg(Database::NAME, Database::TABLE_AttributeS);

//     QVERIFY2(katalogue_db.deleteAttributeEntry(3) == true, "Attribute 3 deleted");

//     QSqlQuery query;
//     query.exec(record3Query);
//     query.next();
//     QVERIFY(query.record().value(0) == QStringLiteral(""));
// }

QTEST_MAIN(DatabaseAttributeTest)
#include "databaseAttributeTest.moc"
