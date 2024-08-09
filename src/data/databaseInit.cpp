#include "databaseInit.h"

DatabaseInit::DatabaseInit(QObject *parent)
    : QObject(parent)
{
}

bool DatabaseInit::connectKatalogueDb(QString path)
{
    /*
     * Connect to the katalogue SQLite database and configure the schema, pragma, etc
     */
    db = QSqlDatabase::addDatabase(DatabaseSchema::DATABASE_TYPE);
    db.setDatabaseName(QStringLiteral("%1/%2").arg(path, DatabaseSchema::DATABASE_NAME));
    qDebug() << db.databaseName();

    if (!db.open())
    {
        qDebug() << "DatabaseInit failed to open: " << db.lastError();
        return false;
    }
    else
    {
        QSqlQuery query;

        // Check if the database is empty
        query.exec(QStringLiteral("PRAGMA user_version"));
        query.next();
        int db_version = query.value(0).toInt();
        qDebug() << "database version: " << db_version;

        if (db_version == 0)
        {
            qDebug() << "New database, creating tables";

            if (!this->initializeSchema())
            {
                qDebug() << "Failed to initialize database schema";
                return false;
            }

            query.exec(QStringLiteral("PRAGMA user_version = %1").arg(DatabaseSchema::DATABASE_VERSION));
            query.exec(QStringLiteral("PRAGMA foreign_keys = ON;"));
        }
        else if (db_version > DatabaseSchema::DATABASE_VERSION)
        {
            qDebug() << "DatabaseInit is newer than application";
        }

        // Enable ON CASCADE each time db is open with foreign_key
        query.exec(QStringLiteral("PRAGMA foreign_keys = ON;"));
    }

    return true;
}

bool DatabaseInit::initializeSchema() const
{
    QSqlQuery query;

    if (!query.exec(DatabaseSchema::SCHEMA_ITEMS))
    {
        qDebug() << "Failed to create table: " << DatabaseSchema::TABLE_ITEMS;
        qDebug() << query.lastError();
        return false;
    }

    if (!query.exec(DatabaseSchema::SCHEMA_ATTRIBUTES))
    {
        qDebug() << "Failed to create table: " << DatabaseSchema::TABLE_ATTRIBUTES;
        qDebug() << query.lastError();
        return false;
    }

    if (!query.exec(DatabaseSchema::SCHEMA_EVENTS))
    {
        qDebug() << "Failed to create table: " << DatabaseSchema::TABLE_EVENTS;
        qDebug() << query.lastError();
        return false;
    }

    if (!query.exec(DatabaseSchema::SCHEMA_NOTES))
    {
        qDebug() << "Failed to create table: " << DatabaseSchema::TABLE_NOTES;
        qDebug() << query.lastQuery();
        qDebug() << query.lastError();
        return false;
    }

    if (!query.exec(DatabaseSchema::SCHEMA_TASKS))
    {
        qDebug() << "Failed to create table: " << DatabaseSchema::TABLE_TASKS;
        qDebug() << query.lastError();
        return false;
    }

    return true;
}
