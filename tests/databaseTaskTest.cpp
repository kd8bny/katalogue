#include <QtTest>

#include "data/database.h"

class DatabaseTaskTest : public QObject
{
    Q_OBJECT
    const QString testDBPath = QStringLiteral("../tests/");
    const QString DATABASE_NAME = QStringLiteral("katalogue.db");

private Q_SLOTS:
    void insertTaskEntry() const;
    void updateTaskEntry() const;
    void deleteTaskEntry() const;
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

        QVERIFY(query.value(0).toInt() == taskId);                     // id
        QVERIFY(query.value(1).toString() == tasks.value(i).value(0)); // Due Date
        QVERIFY(query.value(2).toString() == tasks.value(i).value(1)); // Status
        QVERIFY(query.value(3).toString() == tasks.value(i).value(2)); // Title
        QVERIFY(query.value(4).toString() == tasks.value(i).value(3)); // Description
        QVERIFY(query.value(5).toInt() == tasks.value(i).value(4));    // parent
    }
}

void DatabaseTaskTest::updateTaskEntry() const
{
    /*Test Task Insert*/
    Database katalogue_db;
    bool DB_OPEN = katalogue_db.connectKatalogueDb(this->testDBPath);
    qDebug() << "db open" << DB_OPEN;
    QVERIFY(DB_OPEN == true);

    Task refTask(-1);
    refTask.setDueDate(QStringLiteral("2024-09-19T00:00:00.000-05:00"));
    refTask.setStatus(QStringLiteral("refStatus"));
    refTask.setTitle(QStringLiteral("refTitle"));
    refTask.setDescription(QStringLiteral("refDescription"));
    refTask.setItemId(1);

    QVERIFY2(katalogue_db.insertTaskEntry(refTask) == true, "Reference Task added");

    QVariantList taskFields = {
        QStringLiteral("2222-09-19T00:00:00.000-05:00"),
        QStringLiteral("refStatusUpdated"),
        QStringLiteral("refTitleUpdated"),
        QStringLiteral("refDescriptionUpdated"),
        4,
    };

    // Query for reference task id 3
    const QString record3Query = QStringLiteral("SELECT %1, %2, %3, %4, %5 FROM %6 WHERE id=4")
                                     .arg(Database::DUE_DATE, Database::STATUS, Database::TITLE,
                                          Database::DESCRIPTION, Database::KEY_ITEM_ID, Database::TABLE_TASKS);
    QSqlQuery query;
    query.exec(record3Query);
    query.next();
    qDebug() << query.record();

    // Build Task 3
    Task task3(4); // TODO task X
    task3.setDueDate(query.value(0).toString());
    task3.setStatus(query.value(1).toString());
    task3.setTitle(query.value(2).toString());
    task3.setDescription(query.value(3).toString());
    task3.setItemId(query.value(4).toInt());

    // Start Test
    // Update Due Date
    task3.setDueDate(taskFields.value(0).toString());
    QVERIFY(katalogue_db.updateTaskEntry(task3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(0).toString() == taskFields.value(0).toString());

    // Update Status
    task3.setStatus(taskFields.value(1).toString());
    QVERIFY(katalogue_db.updateTaskEntry(task3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(1).toString() == taskFields.value(1).toString());

    // Update Title
    task3.setTitle(taskFields.value(2).toString());
    QVERIFY(katalogue_db.updateTaskEntry(task3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(2).toString() == taskFields.value(2).toString());

    // Update Description
    task3.setDescription(taskFields.value(3).toString());
    QVERIFY(katalogue_db.updateTaskEntry(task3) == true);

    query.exec(record3Query);
    query.next();
    QVERIFY(query.value(3).toString() == taskFields.value(3).toString());
}

void DatabaseTaskTest::deleteTaskEntry() const
{
    Database katalogue_db;
    // Get Data from Record 3
    // Check if KEY is null after delete
    const QString record3Query = QStringLiteral("SELECT %1 FROM %2 WHERE id=3")
                                     .arg(Database::KEY, Database::TABLE_ATTRIBUTES);

    QVERIFY2(katalogue_db.deleteTaskEntry(3) == true, "Task 3 deleted");

    QSqlQuery query;
    query.exec(record3Query);
    query.next();
    QVERIFY(query.record().value(0) == QStringLiteral(""));
}

QTEST_MAIN(DatabaseTaskTest)
#include "databaseTaskTest.moc"
