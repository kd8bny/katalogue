
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
