#include "eventModel.h"

EventModel::EventModel(QObject *parent) : QSqlQueryModel(parent)
{
    QObject::connect(this, &EventModel::setItemId, this, &EventModel::onSetItemId);
    QObject::connect(this, &EventModel::setSortRole, this, &EventModel::onSetSortRole);
    QObject::connect(this, &EventModel::setSortOrder, this, &EventModel::onSetSortOrder);
    QObject::connect(this, &EventModel::modelQueryChanged, this, &EventModel::onModelQueryChanged);

    // QSortFilterProxy requires a reimplementation to sort. I figure let the db do the work
    // Here we hold a reference to a proxy model we can return to the view for filtering
    this->filterProxyModel = new FilterProxyModel;

    Q_EMIT modelQueryChanged();
}

// The method for obtaining data from the model
QVariant EventModel::data(const QModelIndex &index, int role) const
{

    // Define the column number, on the role of number
    int columnId = role - Qt::UserRole - 1;
    // Create the index using a column ID
    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> EventModel::roleNames() const
{

    QHash<int, QByteArray> roles;
    roles[rID] = "id";
    roles[rDate] = DatabaseSchema::DATE.toUtf8();
    roles[rEvent] = DatabaseSchema::EVENT.toUtf8();
    roles[rCost] = DatabaseSchema::COST.toUtf8();
    roles[rIncrement] = DatabaseSchema::INCREMENT.toUtf8();
    roles[rCategory] = DatabaseSchema::CATEGORY.toUtf8();
    roles[rComment] = DatabaseSchema::COMMENT.toUtf8();
    roles[rItemID] = DatabaseSchema::KEY_ITEM_ID.toUtf8();

    return roles;
}

int EventModel::getId(int row) const
{
    return this->data(this->index(row, 0), rID).toInt();
}

FilterProxyModel *EventModel::getFilterProxyModel()
{
    // QSortFilterProxy requires a reimplementation to sort. I figure let the db do the work
    // Here we hold a reference to a proxy model we can return to the view for filtering
    auto *filterProxyModel = new FilterProxyModel;

    filterProxyModel->setSourceModel(this);
    return filterProxyModel;
}

void EventModel::onSetItemId(QString id)
{
    this->itemId = id;

    Q_EMIT modelQueryChanged();
}

void EventModel::onSetSortRole(SortRole role)
{
    this->sortRole = role;
    Q_EMIT modelQueryChanged();
}

void EventModel::onSetSortOrder(Qt::SortOrder order)
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

void EventModel::onModelQueryChanged()
{

    QString modelQueryBuilder = this->modelQueryBase + QStringLiteral("WHERE %1=%2 ").arg(DatabaseSchema::KEY_ITEM_ID, this->itemId);

    switch (this->sortRole)
    {
    case SortRole::EVENT:
        modelQueryBuilder = modelQueryBuilder + QStringLiteral("ORDER BY %1 %2 ").arg(DatabaseSchema::EVENT, this->sortOrder);
        break;

    case SortRole::CATEGORY:
        modelQueryBuilder = modelQueryBuilder + QStringLiteral("ORDER BY %1 %2 ").arg(DatabaseSchema::CATEGORY, this->sortOrder);
        break;

    default:
        this->sortOrder = DatabaseSchema::ORDER_DESC;
        modelQueryBuilder = modelQueryBuilder + QStringLiteral("ORDER BY %1 %2 ").arg(DatabaseSchema::DATE, this->sortOrder);
        break;
    }

    this->setQuery(modelQueryBuilder);
    qDebug() << modelQueryBuilder;

    // Set the filter proxy
    this->filterProxyModel->setSourceModel(this);
}
