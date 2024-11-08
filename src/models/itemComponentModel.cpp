#include "itemComponentModel.h"

ItemComponentModel::ItemComponentModel() : ItemModel()
{
    QObject::connect(this, &ItemComponentModel::filterParentItemId, this, &ItemComponentModel::setParentItemQuery);
}

ItemComponentModel::~ItemComponentModel()
{
}

void ItemComponentModel::setParentItemQuery(int itemId)
{
    qDebug() << itemId;
    const QString modelQueryParentItem = QStringLiteral(" WHERE %1=%2 AND %3 IS 0")
                                             .arg(DatabaseSchema::KEY_ITEM_ID, QStringLiteral("%1").arg(itemId), DatabaseSchema::ARCHIVED);

    this->modelQuery = this->modelQueryBase + modelQueryParentItem;
    Q_EMIT modelQueryChanged();
}
