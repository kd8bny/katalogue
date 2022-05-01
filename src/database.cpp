
#include <sqlite3.h>

class Database
{
private:
    sqlite3* DB;
    const char* db = "test.db";

public:
    Database();
    ~Database();
    int openDatabase();
};

Database::Database()
{
}

Database::~Database()
{
}

int Database::openDatabase()
{
    int r = sqlite3_open(db, &DB);
    return r;
}
