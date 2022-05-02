#include <sqlite3.h>
#include <QObject>

class Database : public QObject
{
    Q_OBJECT

private:
    sqlite3* DB;
    const char* db = "test.db";

public:
    explicit Database(QObject *parent = nullptr);
    ~Database();
    int openDatabase();
};
