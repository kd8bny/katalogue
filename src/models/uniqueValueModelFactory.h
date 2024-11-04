#include <QObject>

#include "uniqueValueModel.h"

#ifndef UNIQUE_VALUE_MODEL_FACTORY_H
#define UNIQUE_VALUE_MODEL_FACTORY_H

class UniqueValueModelFactory : public QObject
{
    Q_OBJECT

public:
    UniqueValueModelFactory() = default;
    ~UniqueValueModelFactory() override = default;

    Q_INVOKABLE UniqueValueModel *createAttributeCategoryModel(QString itemType) const
    {
        auto *uniqueValueModel = new UniqueValueModel();
        uniqueValueModel->setModelQuery(QStringLiteral("SELECT DISTINCT %1 FROM %2 WHERE %3!=%4")
                                            .arg(DatabaseSchema::CATEGORY, DatabaseSchema::TABLE_ATTRIBUTES,
                                                 DatabaseSchema::TYPE, itemType));
        return uniqueValueModel;
    };

    Q_INVOKABLE UniqueValueModel *createEventCategoryModel(QString itemType) const
    {
        auto *uniqueValueModel = new UniqueValueModel();
        uniqueValueModel->setModelQuery(QStringLiteral("SELECT DISTINCT %1 FROM %2 WHERE %3!=%4")
                                            .arg(DatabaseSchema::CATEGORY, DatabaseSchema::TABLE_EVENTS,
                                                 DatabaseSchema::TYPE, itemType));
        return uniqueValueModel;
    };

    Q_INVOKABLE UniqueValueModel *createEventNameModel(QString itemType) const
    {
        auto *uniqueValueModel = new UniqueValueModel();
        uniqueValueModel->setModelQuery(QStringLiteral("SELECT DISTINCT %1 FROM %2 WHERE %3!=%4")
                                            .arg(DatabaseSchema::EVENT, DatabaseSchema::TABLE_EVENTS,
                                                 DatabaseSchema::TYPE, itemType));
        return uniqueValueModel;
    };

    Q_INVOKABLE UniqueValueModel *createEventServicerModel(QString itemType) const
    {
        auto *uniqueValueModel = new UniqueValueModel();
        uniqueValueModel->setModelQuery(QStringLiteral("SELECT DISTINCT %1 FROM %2 WHERE %3!=%4")
                                            .arg(DatabaseSchema::SERVICER, DatabaseSchema::TABLE_EVENTS,
                                                 DatabaseSchema::TYPE, itemType));
        return uniqueValueModel;
    };

    Q_INVOKABLE UniqueValueModel *createItemTypeModel() const
    {
        auto *uniqueValueModel = new UniqueValueModel();
        uniqueValueModel->setModelQuery(QStringLiteral("SELECT DISTINCT %1 FROM %2")
                                            .arg(DatabaseSchema::TYPE, DatabaseSchema::TABLE_ITEMS));
        return uniqueValueModel;
    };

    Q_INVOKABLE UniqueValueModel *createItemParentNameModel(int itemId) const
    {
        auto *uniqueValueModel = new UniqueValueModel();
        // Ignore components and archived Items
        uniqueValueModel->setModelQuery(QStringLiteral("SELECT DISTINCT %1, id FROM %2 WHERE "
                                                       "%3 IS NULL AND %4 IS 0 AND id!=%5")
                                            .arg(DatabaseSchema::NAME, DatabaseSchema::TABLE_ITEMS,
                                                 DatabaseSchema::KEY_ITEM_ID, DatabaseSchema::ARCHIVED,
                                                 QString::number(itemId)));
        return uniqueValueModel;
    };
};

#endif
