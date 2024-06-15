// #include <catch2/catch_test_macros.hpp>

#include "../src/data/database.h"
#include <QtTest>

class DatabaseTest: public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void databaseConnected();

};

// void DatabaseTest::databaseConnected()
// {
//     // QString str = "Hello";
//     QVERIFY(true == true);
// }

void DatabaseTest::databaseConnected()
{
    /*Test Database creation*/

    Database test_db;
    bool DB_OPEN = test_db.connect(QStringLiteral("."));
    qDebug() << "db open" << DB_OPEN;

    QVERIFY(DB_OPEN == true);
}

// TEST_CASE() {
//
//     /
// }

QTEST_MAIN(DatabaseTest)
#include "databaseTest.moc"
