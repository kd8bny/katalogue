#include <catch2/catch_test_macros.hpp>

#include "../src/data/database.h"

TEST_CASE("Test Database creation") {
    Database test_db;
    bool DB_OPEN = test_db.connect(QStringLiteral("."));
    qDebug() << "db open" << DB_OPEN;

    REQUIRE_FALSE(DB_OPEN);
}
