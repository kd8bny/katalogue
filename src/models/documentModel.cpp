#include "documentModel.h"

DocumentModel::DocumentModel(QObject *parent) : QSqlQueryModel(parent)
{
    QObject::connect(this, &DocumentModel::setItemId, this, &DocumentModel::onSetItemId);
    QObject::connect(this, &DocumentModel::setSortRole, this, &DocumentModel::onSetSortRole);
    QObject::connect(this, &DocumentModel::setSortOrder, this, &DocumentModel::onSetSortOrder);
    QObject::connect(this, &DocumentModel::modelQueryChanged, this, &DocumentModel::onModelQueryChanged);

    Q_EMIT modelQueryChanged();
}

// The method for obtaining data from the model
QVariant DocumentModel::data(const QModelIndex &index, int role) const
{

    // Define the column number, on the role of number
    int columnId = role - Qt::UserRole - 1;
    // Create the index using a column ID
    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> DocumentModel::roleNames() const
{

    QHash<int, QByteArray> roles;
    roles[rID] = DatabaseSchema::ID.toUtf8();
    roles[rName] = DatabaseSchema::NAME.toUtf8();
    roles[rFileName] = DatabaseSchema::FILE_NAME.toUtf8();

    return roles;
}

int DocumentModel::getId(int row) const
{
    return this->data(this->index(row, 0), rID).toInt();
}

FilterProxyModel *DocumentModel::getFilterProxyModel()
{
    // QSortFilterProxy requires a reimplementation to sort. I figure let the db do the work
    // Here we hold a reference to a proxy model we can return to the view for filtering
    auto *filterProxyModel = new FilterProxyModel;

    filterProxyModel->setSourceModel(this);
    return filterProxyModel;
}

void DocumentModel::onSetItemId(QString id)
{
    this->itemId = id;

    Q_EMIT modelQueryChanged();
}

void DocumentModel::onSetSortRole(SortRole role)
{
    this->sortRole = role;
    Q_EMIT modelQueryChanged();
}

void DocumentModel::onSetSortOrder(Qt::SortOrder order)
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

void DocumentModel::onModelQueryChanged()
{

    QString modelQueryBuilder = this->modelQueryBase;
    if (!this->itemId.isEmpty())
        modelQueryBuilder += QStringLiteral("WHERE %1=%2 ").arg(DatabaseSchema::KEY_ITEM_ID, this->itemId);

    switch (this->sortRole)
    {
    case SortRole::FILENAME:
        modelQueryBuilder = modelQueryBuilder + QStringLiteral("ORDER BY %1 %2 ").arg(DatabaseSchema::FILE_NAME, this->sortOrder);
        break;

    default:
        this->sortOrder = DatabaseSchema::ORDER_DESC;
        modelQueryBuilder = modelQueryBuilder + QStringLiteral("ORDER BY %1 %2 ").arg(DatabaseSchema::NAME, this->sortOrder);
        break;
    }

    this->setQuery(modelQueryBuilder);
    qDebug() << modelQueryBuilder;
}
