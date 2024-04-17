#include <QObject>
#include <QSqlQueryModel>

#include "itemModel.h"

#ifndef ITEM_COMPONENT_MODEL_H
#define ITEM_COMPONENT_MODEL_H

class ItemComponentModel: public ItemModel
{
    Q_OBJECT

public:
    explicit ItemComponentModel();
    ~ItemComponentModel();

private:
    const QString modelQueryBase = QStringLiteral("SELECT id, %1, %2, %3, %4, %5, %6 FROM %7 ").arg(
            Database::NAME, Database::MAKE, Database::MODEL, Database::YEAR, Database::TYPE, Database::ARCHIVED, Database::TABLE_ITEMS);

Q_SIGNALS:
    void filterParentItemId(int itemId);

public Q_SLOTS:
    void setParentItemQuery(int itemId);
};

#endif
