#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>

#include "databaseSchema.h"
#include "databaseUtils.h"
#include "entryFactory.h"
#include "item.h"

#ifndef ITEM_DATABASE_H
#define ITEM_DATABASE_H

class ItemDatabase final : public QObject
{
    Q_OBJECT

public:
    explicit ItemDatabase(QObject *parent = nullptr);
    ~ItemDatabase() override = default;

    Q_INVOKABLE bool insertEntry(const Item *item) const;
    Q_INVOKABLE bool updateEntry(const Item *item) const;
    Q_INVOKABLE bool deleteEntryById(const int id) const;
    Q_INVOKABLE Item *getEntryById(const int id) const;
    Q_INVOKABLE Item *getNewEntry() const;
    Q_INVOKABLE bool setArchived(const int id, const bool archived) const;
    Q_INVOKABLE bool setUserOrder(const int id, const int user_order) const;
};

#endif
