#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>

#include "databaseSchema.h"
#include "databaseUtils.h"
#include "entries/attribute.h"

#ifndef ATTRIBUTE_DATABASE_H
#define ATTRIBUTE_DATABASE_H

class AttributeDatabase : public QObject
{
    Q_OBJECT

public:
    explicit AttributeDatabase(QObject *parent = nullptr);
    ~AttributeDatabase() override = default;

    bool insertEntry(const Attribute &attribute) const;
    bool updateEntry(const Attribute &attribute) const;
    bool deleteEntryById(const int id) const;
};

#endif
