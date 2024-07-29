#include <QtTest>

#include "data/database.h"

class DatabaseEventTest : public QObject
{
    Q_OBJECT
    const QString testDBPath = QStringLiteral("../tests/");
    const QString DATABASE_NAME = QStringLiteral("katalogue.db");

private Q_SLOTS:
    void insertEventEntry() const;
    // void updateEventEntry() const;
    // void deleteEventEntry() const;
};

/*
 * Database CRUD Tests
 */

void DatabaseEventTest::insertEventEntry() const
{
    /*Test Event Insert*/
    Database katalogue_db;
    bool DB_OPEN = katalogue_db.connectKatalogueDb(this->testDBPath);
    qDebug() << "db open" << DB_OPEN;

    QVERIFY(DB_OPEN == true);

    QList<QVariantList> events;

    // date, event, cost, odometer, category, comment itemId
    // Normal
    events.append(QVariantList({QStringLiteral("2015-09-19T00:00:00.000-05:00"), QStringLiteral("event"),
                                QStringLiteral("123.45"), QStringLiteral("123456.7"), QStringLiteral("category"),
                                QStringLiteral("comment"), 1}));
    // Minimum input
    events.append(QVariantList({QStringLiteral("2015-09-19T00:00:00.000-05:00"), QStringLiteral("eventMin"),
                                QStringLiteral("0.00"), QStringLiteral("0.0"),
                                QStringLiteral(""), QStringLiteral(""), 1}));

    // Inserts correctly
    for (int i = 0; i < events.length(); i++)
    {
        QVariantList eventAsList;
        eventAsList = events.value(i);

        Event event(-1);
        event.setDate(eventAsList.value(0).toString());
        event.setEvent(eventAsList.value(1).toString());
        event.setCost(eventAsList.value(2).toFloat());
        event.setIncrement(eventAsList.value(3).toFloat());
        event.setCategory(eventAsList.value(4).toString());
        event.setComment(eventAsList.value(5).toString());
        event.setItemId(eventAsList.value(6).toInt());

        QVERIFY(katalogue_db.insertEventEntry(event) == true);
    }

    // Validate data
    const QString modelQueryBase = QStringLiteral("SELECT id, %1, %2, %3, %4, %5, %6, %7 FROM %8")
                                       .arg(Database::DATE, Database::EVENT, Database::COST, Database::INCREMENT,
                                            Database::CATEGORY, Database::COMMENT, Database::KEY_ITEM_ID,
                                            Database::TABLE_EVENTS);

    QSqlQuery query;
    query.exec(modelQueryBase);
    for (int i = 0; i < events.length(); i++)
    {
        int eventId = i + 1;

        query.next();
        qDebug() << query.record();

        QVERIFY(query.value(0).toInt() == eventId);                                               // id
        QVERIFY(query.value(1).toString() == events.value(i).value(0));                           // date
        QVERIFY(query.value(2).toString() == events.value(i).value(1));                           // event
        QVERIFY(QString::asprintf("%.2f", query.value(3).toFloat()) == events.value(i).value(2)); // cost
        QVERIFY(QString::asprintf("%.1f", query.value(4).toFloat()) == events.value(i).value(3)); // increment
        QVERIFY(query.value(5).toString() == events.value(i).value(4));                           // category
        QVERIFY(query.value(6).toString() == events.value(i).value(5));                           // comment
        QVERIFY(query.value(7).toInt() == 1);                                                     // parent
    }
}

// void DatabaseEventTest::updateEventEntry() const
// {
//     /*Test Event Insert*/
//     Database katalogue_db;
//     bool DB_OPEN = katalogue_db.connectKatalogueDb(this->testDBPath);
//     qDebug() << "db open" << DB_OPEN;
//     QVERIFY(DB_OPEN == true);

//     Event refEvent(-1);
//     refEvent.setKey(QStringLiteral("refKey"));
//     refEvent.setValue(QStringLiteral("refValue"));
//     refEvent.setCategory(QStringLiteral("refCat"));
//     refEvent.setItemId(1);

//     QVERIFY2(katalogue_db.insertEventEntry(refEvent) == true, "Reference Event added");

//     QVariantList eventFields = {
//         QStringLiteral("refKeyUpdated"),
//         QStringLiteral("refValueUpdated"),
//         QStringLiteral("refCatUpdated"),
//         4,
//     };

//     // Query for reference event id 3
//     const QString record3Query = QStringLiteral("SELECT %1, %2, %3, %4 FROM %5 WHERE id=3")
//                                      .arg(Database::KEY, Database::VALUE, Database::CATEGORY,
//                                           Database::KEY_ITEM_ID, Database::TABLE_ATTRIBUTES);
//     QSqlQuery query;
//     query.exec(record3Query);
//     query.next();
//     // qDebug() << query.record();

//     // Build Event 3
//     Event event3(3);
//     event3.setKey(query.value(0).toString());
//     event3.setValue(query.value(1).toString());
//     event3.setCategory(query.value(2).toString());
//     event3.setItemId(query.value(3).toInt());

//     // Start Test
//     // Update Key
//     event3.setKey(eventFields.value(0).toString());
//     QVERIFY(katalogue_db.updateEventEntry(event3) == true);

//     query.exec(record3Query);
//     query.next();
//     QVERIFY(query.value(0).toString() == eventFields.value(0).toString());

//     // Update Value
//     event3.setValue(eventFields.value(1).toString());
//     QVERIFY(katalogue_db.updateEventEntry(event3) == true);

//     query.exec(record3Query);
//     query.next();
//     QVERIFY(query.value(1).toString() == eventFields.value(1).toString());

//     // Update Category
//     event3.setCategory(eventFields.value(2).toString());
//     QVERIFY(katalogue_db.updateEventEntry(event3) == true);

//     query.exec(record3Query);
//     query.next();
//     QVERIFY(query.value(2).toString() == eventFields.value(2).toString());
// }

// void DatabaseEventTest::deleteEventEntry() const
// {
//     Database katalogue_db;
//     // Get Data from Record 3
//     // Check if KEY is null after delete
//     const QString record3Query = QStringLiteral("SELECT %1 FROM %2 WHERE id=3")
//                                      .arg(Database::KEY, Database::TABLE_ATTRIBUTES);

//     QVERIFY2(katalogue_db.deleteEventEntry(3) == true, "Event 3 deleted");

//     QSqlQuery query;
//     query.exec(record3Query);
//     query.next();
//     QVERIFY(query.record().value(0) == QStringLiteral(""));
// }

QTEST_MAIN(DatabaseEventTest)
#include "databaseEventTest.moc"
