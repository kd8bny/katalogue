#include <QObject>

#include "itemModel.h"

#ifndef ITEM_MODEL_FACTORY_H
#define ITEM_MODEL_FACTORY_H

class ItemModelFactory : public QObject
{
    Q_OBJECT

public:
    ItemModelFactory() = default;
    ~ItemModelFactory() override = default;

    Q_INVOKABLE ItemModel *createItemModel() const
    {
        auto *itemModel = new ItemModel(
            ItemModel::ItemModelType::ITEMS,
            this->itemsQuery + QStringLiteral("WHERE %1 IS 0 ").arg(DatabaseSchema::ARCHIVED));

        return itemModel;
    };

    Q_INVOKABLE ItemModel *createItemComponentModel(int itemId) const
    {
        auto *itemModel = new ItemModel(
            ItemModel::ItemModelType::COMPONENTS,
            this->itemsQuery + QStringLiteral("WHERE %1 IS 0 AND %2=%3 ")
                                   .arg(DatabaseSchema::ARCHIVED, DatabaseSchema::KEY_ITEM_ID,
                                        QString::number(itemId)));

        return itemModel;
    };

    Q_INVOKABLE ItemModel *createItemArchivedModel() const
    {
        auto *itemModel = new ItemModel(
            ItemModel::ItemModelType::ARCHIVE,
            this->itemsQuery + QStringLiteral("WHERE %1 IS 1 ").arg(DatabaseSchema::ARCHIVED));

        return itemModel;
    };

private:
    QString itemsQuery = QStringLiteral(
                             "SELECT id, %1, %2, %3, %4, %5, %6, %7, %8 FROM %9 ")
                             .arg(DatabaseSchema::NAME, DatabaseSchema::MAKE,
                                  DatabaseSchema::MODEL, DatabaseSchema::YEAR, DatabaseSchema::TYPE,
                                  DatabaseSchema::ARCHIVED, DatabaseSchema::USER_ORDER, DatabaseSchema::KEY_ITEM_ID,
                                  DatabaseSchema::TABLE_ITEMS);
};

#endif
