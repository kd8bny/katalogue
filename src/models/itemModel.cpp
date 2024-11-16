#include "itemModel.h"

ItemModel::ItemModel(ItemModelType itemModelType, QString modelQueryBase, QObject *parent) : QSqlQueryModel(parent)
{
    QObject::connect(this, &ItemModel::setItemPosition, this, &ItemModel::onSetItemPosition);
    QObject::connect(this, &ItemModel::setSortRole, this, &ItemModel::onSetSortRole);
    QObject::connect(this, &ItemModel::setSortOrder, this, &ItemModel::onSetSortOrder);
    QObject::connect(this, &ItemModel::toggleComponents, this, &ItemModel::onToggleComponents);
    QObject::connect(this, &ItemModel::modelQueryChanged, this, &ItemModel::onModelQueryChanged);

    this->itemModelType = itemModelType;
    this->modelQueryBase = modelQueryBase;

    // QSortFilterProxy requires a reimplementation to sort. I figure let the db do the work
    // Here we hold a reference to a proxy model we can return to the view for filtering
    this->filterProxyModel = new FilterProxyModel;

    if (itemModelType == ItemModelType::COMPONENTS)
        includeComponent = true;

    Q_EMIT modelQueryChanged();
}

QVariant ItemModel::data(const QModelIndex &index, int role) const
{

    // Define the column number, on the role of number
    int columnId = role - Qt::UserRole - 1;
    // Create the index using a column ID
    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> ItemModel::roleNames() const
{

    QHash<int, QByteArray> roles;
    roles[rID] = DatabaseSchema::ID.toUtf8();
    roles[rNAME] = DatabaseSchema::NAME.toUtf8();
    roles[rMAKE] = DatabaseSchema::MAKE.toUtf8();
    roles[rMODEL] = DatabaseSchema::MODEL.toUtf8();
    roles[rYEAR] = DatabaseSchema::YEAR.toUtf8();
    roles[rTYPE] = DatabaseSchema::TYPE.toUtf8();
    roles[rARCHIVED] = DatabaseSchema::ARCHIVED.toUtf8();
    roles[rUSER_ORDER] = DatabaseSchema::USER_ORDER.toUtf8();
    roles[rITEM_ID] = DatabaseSchema::KEY_ITEM_ID.toUtf8();

    return roles;
}

int ItemModel::getId(int row) const
{
    return this->data(this->index(row, 0), rID).toInt();
}

void ItemModel::onSetItemPosition(const int index, const int direction)
{
    ItemDatabase db;

    for (int i = 0; i < this->rowCount(); i++)
    {
        int id = this->data(this->index(index, 0), rID).toInt();
        if (direction == 1)
        {
            // Move down in list by increasing value
            db.setUserOrder(id, index + 1);

            // Move affected following Item
            int pid = this->data(this->index(index + 1, 0), rID).toInt();
            db.setUserOrder(pid, index - 1);
        }
        else if (direction == -1)
        {
            // Move up in list by increasing value
            db.setUserOrder(id, index);

            // Move affected preceding Item
            int pid = this->data(this->index(index - 1, 0), rID).toInt();
            db.setUserOrder(pid, index + 1);
        }
    }

    Q_EMIT modelQueryChanged();
}

void ItemModel::onSetSortRole(SortRole role)
{
    this->sortRole = role;
    Q_EMIT modelQueryChanged();
}

void ItemModel::onSetSortOrder(Qt::SortOrder order)
{
    switch (order)
    {
    case Qt::SortOrder::AscendingOrder:
        this->sortOrder = DatabaseSchema::ORDER_ASC;
        Q_EMIT modelQueryChanged();
        break;
    case Qt::SortOrder::DescendingOrder:
        this->sortOrder = DatabaseSchema::ORDER_DESC;
        Q_EMIT modelQueryChanged();
        break;

    default:
        qDebug() << "Invalid sortOrder";
        break;
    }
}

void ItemModel::onToggleComponents()
{
    this->includeComponent = !this->includeComponent;
    Q_EMIT modelQueryChanged();
}

void ItemModel::onModelQueryChanged()
{
    QString modelQueryBuilder = this->modelQueryBase;

    if (!this->includeComponent)
        modelQueryBuilder = modelQueryBuilder + QStringLiteral("AND %1 IS NULL ").arg(DatabaseSchema::KEY_ITEM_ID);

    switch (this->sortRole)
    {
    case SortRole::NAME:
        modelQueryBuilder = modelQueryBuilder + QStringLiteral("ORDER BY %1 %2 ").arg(DatabaseSchema::NAME, this->sortOrder);
        break;

    case SortRole::TYPE:
        modelQueryBuilder = modelQueryBuilder + QStringLiteral("ORDER BY %1 %2 ").arg(DatabaseSchema::TYPE, this->sortOrder);
        break;

    case SortRole::YEAR:
        modelQueryBuilder = modelQueryBuilder + QStringLiteral("ORDER BY %1 %2 ").arg(DatabaseSchema::YEAR, this->sortOrder);
        break;

    default:
        this->sortOrder = DatabaseSchema::ORDER_ASC;
        modelQueryBuilder = modelQueryBuilder + QStringLiteral("ORDER BY %1 %2 NULLS LAST ").arg(DatabaseSchema::USER_ORDER, this->sortOrder);
        break;
    }

    this->setQuery(modelQueryBuilder);

    // Set the filter proxy
    this->filterProxyModel->setSourceModel(this);
}
