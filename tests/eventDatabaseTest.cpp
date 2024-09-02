#include <QtTest>
#include <QSqlRecord>

#include "databaseInit.h"
#include "databaseSchema.h"
#include "eventDatabase.h"

class EventDatabaseTest : public QObject
{
    Q_OBJECT
    const QString testDBPath = QStringLiteral("../tests/");
    const QString DATABASE_NAME = QStringLiteral("katalogue.db");

private Q_SLOTS:
    void insertEventEntry() const;
    void updateEntry() const;
    void deleteEventEntry() const;
};

/*
 * Database CRUD Tests
 */

void EventDatabaseTest::insertEventEntry() const
{
    /*Test Event Insert*/
    DatabaseInit init_db;
    bool DB_OPEN = init_db.connectKatalogueDb(this->testDBPath);
    QVERIFY2(DB_OPEN == true, "db open");

    EventDatabase katalogue_db;

    QVERIFY(DB_OPEN == true);

    QList<QVariantList> events;

    // date, event, cost, odometer, category, comment itemId
    // Normal
    events.append(QVariantList({QStringLiteral("2015-09-19T00:00:00.000-05:00"), QStringLiteral("event"),
                                QStringLiteral("servicer"), QStringLiteral("123.45"), QStringLiteral("123456.7"),
                                QStringLiteral("category"), QStringLiteral("comment"), 1}));
    // Minimum input
    events.append(QVariantList({QStringLiteral("2015-09-19T00:00:00.000-05:00"), QStringLiteral("eventMin"),
                                QStringLiteral(""), QStringLiteral("0.00"), QStringLiteral("0.0"),
                                QStringLiteral(""), QStringLiteral(""), 1}));

    // Inserts correctly
    for (int i = 0; i < events.length(); i++)
    {
        EntryFactory entryFactory;
        Event *event = entryFactory.createEvent();

        QVariantList eventAsList;
        eventAsList = events.value(i);

        event->setDate(eventAsList.value(0).toString());
        event->setEvent(eventAsList.value(1).toString());
        event->setServicer(eventAsList.value(2).toString());
        event->setCost(eventAsList.value(3).toFloat());
        event->setIncrement(eventAsList.value(4).toFloat());
        event->setCategory(eventAsList.value(5).toString());
        event->setComment(eventAsList.value(6).toString());
        event->setItemId(eventAsList.value(7).toInt());

        QVERIFY(katalogue_db.insertEntry(event) == true);

        delete event;
    }

    // Validate data
    const QString modelQueryBase = QStringLiteral("SELECT id, %1, %2, %3, %4, %5, %6, %7, %8 FROM %9")
                                       .arg(DatabaseSchema::DATE, DatabaseSchema::EVENT, DatabaseSchema::SERVICER,
                                            DatabaseSchema::COST, DatabaseSchema::INCREMENT, DatabaseSchema::CATEGORY,
                                            DatabaseSchema::COMMENT, DatabaseSchema::KEY_ITEM_ID,
                                            DatabaseSchema::TABLE_EVENTS);

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
        QVERIFY(query.value(3).toString() == events.value(i).value(2));                           // servicer
        QVERIFY(QString::asprintf("%.2f", query.value(4).toFloat()) == events.value(i).value(3)); // cost
        QVERIFY(QString::asprintf("%.1f", query.value(5).toFloat()) == events.value(i).value(4)); // increment
        QVERIFY(query.value(6).toString() == events.value(i).value(5));                           // category
        QVERIFY(query.value(7).toString() == events.value(i).value(6));                           // comment
        QVERIFY(query.value(8).toInt() == 1);                                                     // parent
    }
}

void EventDatabaseTest::updateEntry() const
{
    /*Test Event Insert*/
    DatabaseInit init_db;
    bool DB_OPEN = init_db.connectKatalogueDb(this->testDBPath);
    QVERIFY2(DB_OPEN == true, "db open");

    EventDatabase katalogue_db;

    EntryFactory entryFactory;
    Event *refEvent = entryFactory.createEvent();

    refEvent->setDate(QStringLiteral("2015-09-19T00:00:00.000-05:00"));
    refEvent->setEvent(QStringLiteral("refEvent"));
    refEvent->setServicer(QStringLiteral("refServicer"));
    refEvent->setCost(123.45);
    refEvent->setIncrement(123456.7);
    refEvent->setCategory(QStringLiteral("category"));
    refEvent->setComment(QStringLiteral("comment"));
    refEvent->setItemId(1);

    QVERIFY2(katalogue_db.insertEntry(refEvent) == true, "Reference Event added");

    delete refEvent;

    QVariantList eventFields = {QStringLiteral("2222-09-19T00:00:00.000-05:00"), QStringLiteral("eventUpdated"),
                                QStringLiteral("servicerUpdated"), QStringLiteral("567.89"), QStringLiteral("765432.1"), QStringLiteral("categoryUpdated"),
                                QStringLiteral("commentUpdated"), 1};

    // Query for reference event id 3
    const QString record3Query = QStringLiteral("SELECT %1, %2, %3, %4, %5, %6, %7 FROM %8 WHERE id=3")
                                     .arg(DatabaseSchema::DATE, DatabaseSchema::EVENT, DatabaseSchema::SERVICER,
                                          DatabaseSchema::COST, DatabaseSchema::INCREMENT, DatabaseSchema::CATEGORY,
                                          DatabaseSchema::COMMENT, DatabaseSchema::TABLE_EVENTS);
    QSqlQuery query;
    query.exec(record3Query);
    query.next();

    // Build Event 3
    Event *event3 = entryFactory.createEvent();
    event3->setId(3);
    event3->setDate(query.value(0).toString());
    event3->setEvent(query.value(1).toString());
    event3->setServicer(query.value(2).toString());
    event3->setCost(query.value(3).toFloat());
    event3->setIncrement(query.value(4).toFloat());
    event3->setCategory(query.value(5).toString());
    event3->setComment(query.value(6).toString());
    event3->setItemId(query.value(7).toInt());

    // Start Test
    // Update Date
    event3->setDate(eventFields.value(0).toString());
    QVERIFY(katalogue_db.updateEntry(event3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(0).toString() == eventFields.value(0).toString());

    // Update Event
    event3->setEvent(eventFields.value(1).toString());
    QVERIFY(katalogue_db.updateEntry(event3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(1).toString() == eventFields.value(1).toString());

    // Update Servicer
    event3->setServicer(eventFields.value(2).toString());
    QVERIFY(katalogue_db.updateEntry(event3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(2).toString() == eventFields.value(2).toString());

    // Update Cost
    event3->setCost(eventFields.value(3).toFloat());
    QVERIFY(katalogue_db.updateEntry(event3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(QString::asprintf("%.2f", query.value(3).toFloat()) == eventFields.value(3));

    // Update Increment
    event3->setIncrement(eventFields.value(4).toFloat());
    QVERIFY(katalogue_db.updateEntry(event3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(QString::asprintf("%.1f", query.value(4).toFloat()) == eventFields.value(4));

    // Update Category
    event3->setCategory(eventFields.value(5).toString());
    QVERIFY(katalogue_db.updateEntry(event3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(5).toString() == eventFields.value(5).toString());

    // Update Comment
    event3->setComment(eventFields.value(6).toString());
    QVERIFY(katalogue_db.updateEntry(event3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(6).toString() == eventFields.value(6).toString());

    delete event3;
}

void EventDatabaseTest::deleteEventEntry() const
{
    EventDatabase katalogue_db;
    // Get Data from Record 3
    // Check if KEY is null after delete
    const QString record3Query = QStringLiteral("SELECT %1 FROM %2 WHERE id=3")
                                     .arg(DatabaseSchema::DATE, DatabaseSchema::TABLE_EVENTS);

    QVERIFY2(katalogue_db.deleteEntryById(3) == true, "Event 3 deleted");

    QSqlQuery query;
    query.exec(record3Query);
    query.next();
    QVERIFY(query.record().value(0) == QStringLiteral(""));
}

QTEST_MAIN(EventDatabaseTest)
#include "eventDatabaseTest.moc"
