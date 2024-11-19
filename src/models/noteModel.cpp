#include "noteModel.h"

NoteModel::NoteModel(QObject *parent) : QSqlQueryModel(parent)
{
    QObject::connect(this, &NoteModel::setItemId, this, &NoteModel::onSetItemId);
    QObject::connect(this, &NoteModel::setSortRole, this, &NoteModel::onSetSortRole);
    QObject::connect(this, &NoteModel::setSortOrder, this, &NoteModel::onSetSortOrder);
    QObject::connect(this, &NoteModel::modelQueryChanged, this, &NoteModel::onModelQueryChanged);

    // QSortFilterProxy requires a reimplementation to sort. I figure let the db do the work
    // Here we hold a reference to a proxy model we can return to the view for filtering
    this->filterProxyModel = new FilterProxyModel;

    Q_EMIT modelQueryChanged();
}

// The method for obtaining data from the model
QVariant NoteModel::data(const QModelIndex &index, int role) const
{

    // Define the column number, on the role of number
    int columnId = role - Qt::UserRole - 1;
    // Create the index using a column ID
    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> NoteModel::roleNames() const
{

    QHash<int, QByteArray> roles;
    roles[rID] = "id";
    roles[rTitle] = DatabaseSchema::TITLE.toUtf8();
    roles[rNote] = DatabaseSchema::NOTE_CONTENT.toUtf8();
    roles[rItemID] = DatabaseSchema::KEY_ITEM_ID.toUtf8();

    return roles;
}

int NoteModel::getId(int row) const
{
    return this->data(this->index(row, 0), rID).toInt();
}

void NoteModel::onSetItemId(QString id)
{
    this->itemId = id;

    Q_EMIT modelQueryChanged();
}

void NoteModel::onSetSortRole(SortRole role)
{
    this->sortRole = role;
    Q_EMIT modelQueryChanged();
}

void NoteModel::onSetSortOrder(Qt::SortOrder order)
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

void NoteModel::onModelQueryChanged()
{

    QString modelQueryBuilder = this->modelQueryBase;
    if (!this->itemId.isEmpty())
        modelQueryBuilder += QStringLiteral("WHERE %1=%2 ").arg(DatabaseSchema::KEY_ITEM_ID, this->itemId);

    switch (this->sortRole)
    {
    case SortRole::NOTE_CONTENT:
        modelQueryBuilder = modelQueryBuilder + QStringLiteral("ORDER BY %1 %2 ").arg(DatabaseSchema::NOTE_CONTENT, this->sortOrder);
        break;

    default:
        this->sortOrder = DatabaseSchema::ORDER_DESC;
        modelQueryBuilder = modelQueryBuilder + QStringLiteral("ORDER BY %1 %2 ").arg(DatabaseSchema::TITLE, this->sortOrder);
        break;
    }

    this->setQuery(modelQueryBuilder);
    qDebug() << modelQueryBuilder;

    // Set the filter proxy
    this->filterProxyModel->setSourceModel(this);
}
