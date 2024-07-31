#include <QtTest>

#include "data/database.h"

class DatabaseNoteTest : public QObject
{
    Q_OBJECT
    const QString testDBPath = QStringLiteral("../tests/");
    const QString DATABASE_NAME = QStringLiteral("katalogue.db");

private Q_SLOTS:
    void insertNoteEntry() const;
    // void updateNoteEntry() const;
    // void deleteNoteEntry() const;
};

/*
 * Database CRUD Tests
 */

void DatabaseNoteTest::insertNoteEntry() const
{
    /*Test Note Insert*/
    Database katalogue_db;
    bool DB_OPEN = katalogue_db.connectKatalogueDb(this->testDBPath);
    qDebug() << "db open" << DB_OPEN;

    QVERIFY(DB_OPEN == true);

    QList<QVariantList> notes;

    // QVariantList key, value, category, parent
    // Normal
    notes.append(QVariantList({QStringLiteral("title"), QStringLiteral("note content"), 1}));
    // Note without item
    notes.append(QVariantList({QStringLiteral("title"), QStringLiteral("note content"), 0}));

    // Inserts correctly
    for (int i = 0; i < notes.length(); i++)
    {
        QVariantList noteAsList;
        noteAsList = notes.value(i);

        Note note(-1);
        note.setTitle(noteAsList.value(0).toString());
        note.setNoteContent(noteAsList.value(1).toString());
        note.setItemId(noteAsList.value(2).toInt());

        QVERIFY(katalogue_db.insertNoteEntry(note) == true);
    }

    // Validate data
    const QString modelQueryBase = QStringLiteral("SELECT id, %1, %2, %3 FROM %4 ")
                                       .arg(Database::TITLE, Database::NOTE_CONTENT, Database::KEY_ITEM_ID,
                                            Database::TABLE_NOTES);

    QSqlQuery query;
    query.exec(modelQueryBase);
    for (int i = 0; i < notes.length(); i++)
    {
        int noteId = i + 1;

        query.next();

        QVERIFY(query.value(0).toInt() == noteId);                     // id
        QVERIFY(query.value(1).toString() == notes.value(i).value(0)); // Title
        QVERIFY(query.value(2).toString() == notes.value(i).value(1)); // Content
        qDebug() << notes.value(i).value(2);
        QVERIFY(query.value(3).toInt() == notes.value(i).value(2)); // parent
    }
}

// void DatabaseNoteTest::updateNoteEntry() const
// {
//     /*Test Note Insert*/
//     Database katalogue_db;
//     bool DB_OPEN = katalogue_db.connectKatalogueDb(this->testDBPath);
//     qDebug() << "db open" << DB_OPEN;
//     QVERIFY(DB_OPEN == true);

//     Note refNote(-1);
//     refNote.setKey(QStringLiteral("refKey"));
//     refNote.setValue(QStringLiteral("refValue"));
//     refNote.setCategory(QStringLiteral("refCat"));
//     refNote.setItemId(1);

//     QVERIFY2(katalogue_db.insertNoteEntry(refNote) == true, "Reference Note added");

//     QVariantList noteFields = {
//         QStringLiteral("refKeyUpdated"),
//         QStringLiteral("refValueUpdated"),
//         QStringLiteral("refCatUpdated"),
//         4,
//     };

//     // Query for reference note id 3
//     const QString record3Query = QStringLiteral("SELECT %1, %2, %3, %4 FROM %5 WHERE id=3")
//                                      .arg(Database::KEY, Database::VALUE, Database::CATEGORY,
//                                           Database::KEY_ITEM_ID, Database::TABLE_ATTRIBUTES);
//     QSqlQuery query;
//     query.exec(record3Query);
//     query.next();
//     // qDebug() << query.record();

//     // Build Note 3
//     Note note3(3);
//     note3.setKey(query.value(0).toString());
//     note3.setValue(query.value(1).toString());
//     note3.setCategory(query.value(2).toString());
//     note3.setItemId(query.value(3).toInt());

//     // Start Test
//     // Update Key
//     note3.setKey(noteFields.value(0).toString());
//     QVERIFY(katalogue_db.updateNoteEntry(note3) == true);

//     query.exec(record3Query);
//     query.next();
//     QVERIFY(query.value(0).toString() == noteFields.value(0).toString());

//     // Update Value
//     note3.setValue(noteFields.value(1).toString());
//     QVERIFY(katalogue_db.updateNoteEntry(note3) == true);

//     query.exec(record3Query);
//     query.next();
//     QVERIFY(query.value(1).toString() == noteFields.value(1).toString());

//     // Update Category
//     note3.setCategory(noteFields.value(2).toString());
//     QVERIFY(katalogue_db.updateNoteEntry(note3) == true);

//     query.exec(record3Query);
//     query.next();
//     QVERIFY(query.value(2).toString() == noteFields.value(2).toString());
// }

// void DatabaseNoteTest::deleteNoteEntry() const
// {
//     Database katalogue_db;
//     // Get Data from Record 3
//     // Check if KEY is null after delete
//     const QString record3Query = QStringLiteral("SELECT %1 FROM %2 WHERE id=3")
//                                      .arg(Database::KEY, Database::TABLE_ATTRIBUTES);

//     QVERIFY2(katalogue_db.deleteNoteEntry(3) == true, "Note 3 deleted");

//     QSqlQuery query;
//     query.exec(record3Query);
//     query.next();
//     QVERIFY(query.record().value(0) == QStringLiteral(""));
// }

QTEST_MAIN(DatabaseNoteTest)
#include "databaseNoteTest.moc"
