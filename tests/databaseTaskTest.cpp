#include <QtTest>

#include "data/database.h"

class DatabaseTaskTest : public QObject
{
    Q_OBJECT
    const QString testDBPath = QStringLiteral("../tests/");
    const QString DATABASE_NAME = QStringLiteral("katalogue.db");

private Q_SLOTS:
    void insertTaskEntry() const;
    // void updateTaskEntry() const;
    // void deleteTaskEntry() const;
};

/*
 * Database CRUD Tests
 */

void DatabaseTaskTest::insertTaskEntry() const
{
    /*Test Task Insert*/
    Database katalogue_db;
    bool DB_OPEN = katalogue_db.connectKatalogueDb(this->testDBPath);
    qDebug() << "db open" << DB_OPEN;

    QVERIFY(DB_OPEN == true);

    QList<QVariantList> tasks;

    // QVariantList dueDate, status, title, description, parent
    // Normal
    tasks.append(QVariantList({QStringLiteral("2024-09-19T00:00:00.000-05:00"), QStringLiteral("status"), QStringLiteral("title"), QStringLiteral("description"), 1}));
    // Minimum input
    tasks.append(QVariantList({QStringLiteral("2024-09-19T00:00:00.000-05:00"), QStringLiteral("status2"), QStringLiteral("title2"), QStringLiteral(""), 1}));
    // Note without item
    tasks.append(QVariantList({QStringLiteral("2024-09-19T00:00:00.000-05:00"), QStringLiteral("status3"), QStringLiteral("no parent"), QStringLiteral(""), 0}));

    // qDebug() << tasks;

    // Inserts correctly
    for (int i = 0; i < tasks.length(); i++)
    {
        QVariantList taskAsList;
        taskAsList = tasks.value(i);

        Task task(-1);
        task.setDueDate(taskAsList.value(0).toString());
        task.setStatus(taskAsList.value(1).toString());
        task.setTitle(taskAsList.value(2).toString());
        task.setDescription(taskAsList.value(3).toString());
        task.setItemId(taskAsList.value(4).toInt());

        QVERIFY(katalogue_db.insertTaskEntry(task) == true);
    }

    // Validate data
    const QString modelQueryBase = QStringLiteral("SELECT id, %1, %2, %3, %4, %5 FROM %6 ")
                                       .arg(Database::DUE_DATE, Database::STATUS, Database::TITLE,
                                            Database::DESCRIPTION, Database::KEY_ITEM_ID, Database::TABLE_TASKS);

    QSqlQuery query;
    query.exec(modelQueryBase);
    for (int i = 0; i < tasks.length(); i++)
    {
        int taskId = i + 1;

        query.next();
        // qDebug() << query.record();

        QVERIFY(query.value(0).toInt() == taskId);                     // id
        QVERIFY(query.value(1).toString() == tasks.value(i).value(0)); // Due Date
        QVERIFY(query.value(2).toString() == tasks.value(i).value(1)); // Status
        QVERIFY(query.value(3).toString() == tasks.value(i).value(2)); // Title
        QVERIFY(query.value(4).toString() == tasks.value(i).value(3)); // Description
        QVERIFY(query.value(5).toInt() == tasks.value(i).value(4));    // parent
    }
}

// void DatabaseTaskTest::updateTaskEntry() const
// {
//     /*Test Task Insert*/
//     Database katalogue_db;
//     bool DB_OPEN = katalogue_db.connectKatalogueDb(this->testDBPath);
//     qDebug() << "db open" << DB_OPEN;
//     QVERIFY(DB_OPEN == true);

//     Task refTask(-1);
//     refTask.setKey(QStringLiteral("refKey"));
//     refTask.setValue(QStringLiteral("refValue"));
//     refTask.setCategory(QStringLiteral("refCat"));
//     refTask.setItemId(1);

//     QVERIFY2(katalogue_db.insertTaskEntry(refTask) == true, "Reference Task added");

//     QVariantList taskFields = {
//         QStringLiteral("refKeyUpdated"),
//         QStringLiteral("refValueUpdated"),
//         QStringLiteral("refCatUpdated"),
//         4,
//     };

//     // Query for reference task id 3
//     const QString record3Query = QStringLiteral("SELECT %1, %2, %3, %4 FROM %5 WHERE id=3")
//                                      .arg(Database::KEY, Database::VALUE, Database::CATEGORY,
//                                           Database::KEY_ITEM_ID, Database::TABLE_ATTRIBUTES);
//     QSqlQuery query;
//     query.exec(record3Query);
//     query.next();
//     // qDebug() << query.record();

//     // Build Task 3
//     Task task3(3);
//     task3.setKey(query.value(0).toString());
//     task3.setValue(query.value(1).toString());
//     task3.setCategory(query.value(2).toString());
//     task3.setItemId(query.value(3).toInt());

//     // Start Test
//     // Update Key
//     task3.setKey(taskFields.value(0).toString());
//     QVERIFY(katalogue_db.updateTaskEntry(task3) == true);

//     query.exec(record3Query);
//     query.next();
//     QVERIFY(query.value(0).toString() == taskFields.value(0).toString());

//     // Update Value
//     task3.setValue(taskFields.value(1).toString());
//     QVERIFY(katalogue_db.updateTaskEntry(task3) == true);

//     query.exec(record3Query);
//     query.next();
//     QVERIFY(query.value(1).toString() == taskFields.value(1).toString());

//     // Update Category
//     task3.setCategory(taskFields.value(2).toString());
//     QVERIFY(katalogue_db.updateTaskEntry(task3) == true);

//     query.exec(record3Query);
//     query.next();
//     QVERIFY(query.value(2).toString() == taskFields.value(2).toString());
// }

// void DatabaseTaskTest::deleteTaskEntry() const
// {
//     Database katalogue_db;
//     // Get Data from Record 3
//     // Check if KEY is null after delete
//     const QString record3Query = QStringLiteral("SELECT %1 FROM %2 WHERE id=3")
//                                      .arg(Database::KEY, Database::TABLE_ATTRIBUTES);

//     QVERIFY2(katalogue_db.deleteTaskEntry(3) == true, "Task 3 deleted");

//     QSqlQuery query;
//     query.exec(record3Query);
//     query.next();
//     QVERIFY(query.record().value(0) == QStringLiteral(""));
// }

QTEST_MAIN(DatabaseTaskTest)
#include "databaseTaskTest.moc"
