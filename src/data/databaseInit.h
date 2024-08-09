#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>

#include "databaseSchema.h"
#include "databaseUtils.h"

class DatabaseInit : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseInit(QObject *parent = nullptr);
    ~DatabaseInit() override = default;

    bool connectKatalogueDb(QString path);
    bool initializeSchema() const;

private:
    QSqlDatabase db;
};
