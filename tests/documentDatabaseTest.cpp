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
    void updateEntry() const;
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
    documents.append(QVariantList({QStringLiteral("to delete"), QStringLiteral("filename"), QByteArray("data"), 2, 0, 0}));

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

void DocumentDatabaseTest::updateEntry() const
{
    /*Test Document Insert*/
    DatabaseInit init_db;
    bool DB_OPEN = init_db.connectKatalogueDb(this->testDBPath);
    QVERIFY2(DB_OPEN == true, "db open");

    DocumentDatabase katalogue_db;

    EntryFactory entryFactory;
    Document *refDocument = entryFactory.createDocument();

    refDocument->setName(QStringLiteral("refName"));
    refDocument->setFileName(QStringLiteral("refFileName"));
    refDocument->setDataLazy(QByteArray("refData"));
    refDocument->setItemId(0);
    refDocument->setEventId(0);
    refDocument->setNoteId(0);

    QVERIFY2(katalogue_db.insertEntry(refDocument) != -1, "Reference Document added");

    delete refDocument;

    QVariantList documentFields = {
        QStringLiteral("refNameUpdated"),
        QStringLiteral("refFileNameUpdated"),
        QByteArray("refDataUpdated"),
        1, 1, 1};

    // Query for reference document id 4
    const QString record4Query = QStringLiteral("SELECT %1, %2, %3, %4, %5, %6 FROM %7 WHERE id=4")
                                     .arg(DatabaseSchema::NAME, DatabaseSchema::FILE_NAME, DatabaseSchema::DATA,
                                          DatabaseSchema::KEY_ITEM_ID, DatabaseSchema::KEY_EVENT_ID,
                                          DatabaseSchema::KEY_NOTE_ID, DatabaseSchema::TABLE_DOCUMENTS);
    QSqlQuery query;
    query.exec(record4Query);
    query.next();
    qDebug() << query.record();

    // Build Document 4
    Document *document4 = entryFactory.createDocument();
    document4->setId(4);
    document4->setName(query.value(0).toString());
    document4->setFileName(query.value(1).toString());
    document4->setDataLazy(query.value(2).toByteArray());
    document4->setItemId(query.value(3).toInt());
    document4->setEventId(query.value(4).toInt());
    document4->setNoteId(query.value(5).toInt());

    // Start Test
    // Update Name
    document4->setName(documentFields.value(0).toString());
    QVERIFY(katalogue_db.updateEntry(document4) == true);

    query.exec(record4Query);
    query.next();
    qDebug() << query.value(0).toString() << documentFields.value(0).toString();
    QVERIFY(query.value(0).toString() == documentFields.value(0).toString());

    // Update File Name
    document4->setFileName(documentFields.value(1).toString());
    QVERIFY(katalogue_db.updateEntry(document4) == true);

    query.exec(record4Query);
    query.next();
    QVERIFY(query.value(1).toString() == documentFields.value(1).toString());

    // Update Data
    document4->setDataLazy(documentFields.value(2).toByteArray());
    QVERIFY(katalogue_db.updateEntry(document4) == true);

    query.exec(record4Query);
    query.next();
    QVERIFY(query.value(2).toByteArray() == documentFields.value(2).toByteArray());

    // Update ItemId
    document4->setItemId(documentFields.value(3).toInt());
    QVERIFY(katalogue_db.updateEntry(document4) == true);

    query.exec(record4Query);
    query.next();
    QVERIFY(query.value(3).toInt() == documentFields.value(3).toInt());

    // Update EventId
    document4->setEventId(documentFields.value(4).toInt());
    QVERIFY(katalogue_db.updateEntry(document4) == true);

    query.exec(record4Query);
    query.next();
    QVERIFY(query.value(4).toInt() == documentFields.value(4).toInt());

    // Update NoteId
    document4->setNoteId(documentFields.value(5).toInt());
    QVERIFY(katalogue_db.updateEntry(document4) == true);

    query.exec(record4Query);
    query.next();
    QVERIFY(query.value(5).toInt() == documentFields.value(5).toInt());

    delete document4;
}

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
