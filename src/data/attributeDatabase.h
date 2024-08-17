#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>

#include "databaseSchema.h"
#include "databaseUtils.h"
#include "entryFactory.h"
#include "attribute.h"

#ifndef ATTRIBUTE_DATABASE_H
#define ATTRIBUTE_DATABASE_H

class AttributeDatabase : public QObject
{
    Q_OBJECT

public:
    explicit AttributeDatabase(QObject *parent = nullptr);
    ~AttributeDatabase() override = default;

    Q_INVOKABLE bool insertEntry(const Attribute *attribute) const;
    Q_INVOKABLE bool updateEntry(const Attribute *attribute) const;
    Q_INVOKABLE bool deleteEntryById(const int id) const;
    Q_INVOKABLE Attribute *getEntryById(const int id = 0) const;
};

#endif
