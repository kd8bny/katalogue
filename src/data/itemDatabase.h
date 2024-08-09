#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>

#include "databaseSchema.h"
#include "databaseUtils.h"
#include "entries/item.h"

#ifndef ITEM_DATABASE_H
#define ITEM_DATABASE_H

class ItemDatabase final : public QObject
{
    Q_OBJECT

public:
    explicit ItemDatabase(QObject *parent = nullptr);
    ~ItemDatabase() override = default;

    bool insertEntry(const Item &item) const;
    bool updateEntry(const Item &item) const;
    bool deleteEntryById(const int id) const;

    bool setArchived(const int id, const bool archived) const;
    bool setUserOrder(const int id, const int user_order) const;
};

#endif
