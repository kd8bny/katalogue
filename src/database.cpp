#include <database.h>

Database::Database(QObject *parent)
    : QObject(parent)
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
