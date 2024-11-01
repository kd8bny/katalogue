#include <QtTest>
#include <QSqlRecord>

#include "databaseInit.h"
#include "databaseSchema.h"
#include "documentDatabase.h"

class DocumentDatabaseTest : public QObject
{
    Q_OBJECT
    const QString testDBPath = QStringLiteral("../tests/");
    const QString DATABASE_NAME = QStringLiteral("katalogue.db");

private Q_SLOTS:
    void insertEntry() const;
    // void updateEntry() const;
    void deleteDocumentEntry() const;
};

/*
 * Database CRUD Tests
 */

void DocumentDatabaseTest::insertEntry() const
{
    /*Test Document Insert*/
    DatabaseInit init_db;
    bool DB_OPEN = init_db.connectKatalogueDb(this->testDBPath);
    QVERIFY2(DB_OPEN == true, "db open");

    DocumentDatabase katalogue_db;

    QList<QVariantList> documents;

    // QVariantList name, fileName, data, itemId, eventId, noteId
    // Orphan Document
    documents.append(QVariantList({QStringLiteral("name"), QStringLiteral("filename"), QByteArray("data"), 0, 0, 0}));
    // Item Document
    documents.append(QVariantList({QStringLiteral("name"), QStringLiteral("filename"), QByteArray("data"), 2, 0, 0}));
    // Event Document
    // documents.append(QVariantList({QStringLiteral("name"), QStringLiteral("filename"), QByteArray("data"), 2, 1, 0})); # TODO events and notes db
    // Note Document
    // documents.append(QVariantList({QStringLiteral("name"), QStringLiteral("filename"), QByteArray("data"), 0, 0, 1}));
    // To DELETE
    documents.append(QVariantList({QStringLiteral("name"), QStringLiteral("filename"), QByteArray("data"), 2, 0, 0}));

    // qDebug() << documents;

    // Inserts correctly
    for (int i = 0; i < documents.length(); i++)
    {
        EntryFactory entryFactory;
        Document *document = entryFactory.createDocument();

        QVariantList documentAsList;
        documentAsList = documents.value(i);

        document->setName(documentAsList.value(0).toString());
        document->setFileName(documentAsList.value(1).toString());
        document->setDataLazy(documentAsList.value(2).toByteArray());
        document->setItemId(documentAsList.value(3).toInt());
        document->setEventId(documentAsList.value(4).toInt());
        document->setNoteId(documentAsList.value(5).toInt());

        QVERIFY(katalogue_db.insertEntry(document) != -1);

        delete document;
    }

    // Validate data
    const QString modelQueryBase = QStringLiteral("SELECT id, %1, %2, %3, %4, %5, %6 FROM %7 ")
                                       .arg(DatabaseSchema::NAME, DatabaseSchema::FILE_NAME, DatabaseSchema::DATA,
                                            DatabaseSchema::KEY_ITEM_ID, DatabaseSchema::KEY_EVENT_ID,
                                            DatabaseSchema::KEY_NOTE_ID, DatabaseSchema::TABLE_DOCUMENTS);

    QSqlQuery query;
    query.exec(modelQueryBase);
    for (int i = 0; i < documents.length(); i++)
    {
        int documentId = i + 1;

        query.next();

        QVERIFY(query.value(0).toInt() == documentId);                        // id
        QVERIFY(query.value(1).toString() == documents.value(i).value(0));    // Name
        QVERIFY(query.value(2).toString() == documents.value(i).value(1));    // File Name
        QVERIFY(query.value(3).toByteArray() == documents.value(i).value(2)); // Data
        QVERIFY(query.value(4).toInt() == documents.value(i).value(3));       // ItemId
        QVERIFY(query.value(5).toInt() == documents.value(i).value(4));       // EventId
        QVERIFY(query.value(6).toInt() == documents.value(i).value(5));       // NoteId
    }
}

// void DocumentDatabaseTest::updateEntry() const
// {
//     /*Test Document Insert*/
//     DatabaseInit init_db;
//     bool DB_OPEN = init_db.connectKatalogueDb(this->testDBPath);
//     QVERIFY2(DB_OPEN == true, "db open");

//     DocumentDatabase katalogue_db;

//     EntryFactory entryFactory;
//     Document *refDocument = entryFactory.createDocument();

//     refDocument->setDueDate(QStringLiteral("2024-09-19T00:00:00.000-05:00"));
//     refDocument->setStatus(QStringLiteral("refStatus"));
//     refDocument->setTitle(QStringLiteral("refTitle"));
//     refDocument->setDescription(QStringLiteral("refDescription"));
//     refDocument->setItemId(1);

//     QVERIFY2(katalogue_db.insertEntry(refDocument) == true, "Reference Document added");

//     delete refDocument;

//     QVariantList documentFields = {
//         QStringLiteral("2222-09-19T00:00:00.000-05:00"),
//         QStringLiteral("refStatusUpdated"),
//         QStringLiteral("refTitleUpdated"),
//         QStringLiteral("refDescriptionUpdated"),
//         4,
//     };

//     // Query for reference document id 3
//     const QString record4Query = QStringLiteral("SELECT %1, %2, %3, %4, %5 FROM %6 WHERE id=4")
//                                      .arg(DatabaseSchema::DUE_DATE, DatabaseSchema::STATUS, DatabaseSchema::TITLE,
//                                           DatabaseSchema::DESCRIPTION, DatabaseSchema::KEY_ITEM_ID, DatabaseSchema::TABLE_TASKS);
//     QSqlQuery query;
//     query.exec(record4Query);
//     query.next();
//     qDebug() << query.record();

//     // Build Document 4
//     Document *document4 = entryFactory.createDocument();
//     document4->setId(4);
//     document4->setDueDate(query.value(0).toString());
//     document4->setStatus(query.value(1).toString());
//     document4->setTitle(query.value(2).toString());
//     document4->setDescription(query.value(3).toString());
//     document4->setItemId(query.value(4).toInt());

//     // Start Test
//     // Update Due Date
//     document4->setDueDate(documentFields.value(0).toString());
//     QVERIFY(katalogue_db.updateEntry(document4) == true);

//     query.exec(record4Query);
//     query.next();
//     qDebug() << query.value(0).toString() << documentFields.value(0).toString();
//     QVERIFY(query.value(0).toString() == documentFields.value(0).toString());

//     // Update Status
//     document4->setStatus(documentFields.value(1).toString());
//     QVERIFY(katalogue_db.updateEntry(document4) == true);

//     query.exec(record4Query);
//     query.next();
//     QVERIFY(query.value(1).toString() == documentFields.value(1).toString());

//     // Update Title
//     document4->setTitle(documentFields.value(2).toString());
//     QVERIFY(katalogue_db.updateEntry(document4) == true);

//     query.exec(record4Query);
//     query.next();
//     QVERIFY(query.value(2).toString() == documentFields.value(2).toString());

//     // Update Description
//     document4->setDescription(documentFields.value(3).toString());
//     QVERIFY(katalogue_db.updateEntry(document4) == true);

//     query.exec(record4Query);
//     query.next();
//     QVERIFY(query.value(3).toString() == documentFields.value(3).toString());

//     delete document4;
// }

void DocumentDatabaseTest::deleteDocumentEntry() const
{
    DocumentDatabase katalogue_db;
    // Get Data from Record 3
    // Check if NAME is null after delete
    const QString record3Query = QStringLiteral("SELECT %1 FROM %2 WHERE id=3")
                                     .arg(DatabaseSchema::NAME, DatabaseSchema::TABLE_DOCUMENTS);

    QVERIFY2(katalogue_db.deleteEntryById(3) == true, "Document 3 deleted");

    QSqlQuery query;
    query.exec(record3Query);
    query.next();
    QVERIFY(query.record().value(0) == QStringLiteral(""));
}

QTEST_MAIN(DocumentDatabaseTest)
#include "documentDatabaseTest.moc"
