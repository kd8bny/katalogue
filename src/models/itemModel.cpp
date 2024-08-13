#include "itemModel.h"

ItemModel::ItemModel(QObject *parent) : QSqlQueryModel(parent)
{
    QObject::connect(this, &ItemModel::filterItem, this, &ItemModel::setItemQuery);
    QObject::connect(this, &ItemModel::filterArchive, this, &ItemModel::setArchiveQuery);
    QObject::connect(this, &ItemModel::filterComponent, this, &ItemModel::setComponentQuery);
    QObject::connect(this, &ItemModel::dataChanged, this, &ItemModel::refresh);

    Q_EMIT setItemQuery();
    this->refresh();
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

void ItemModel::setItemPosition(const int index, const int direction) const
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

    Q_EMIT dataChanged();
}

void ItemModel::setItemQuery()
{
    modelQuery = this->modelQueryBase + this->modelQueryParentItem + this->modelQuerySortUser;
    qDebug() << modelQuery;
    Q_EMIT dataChanged();
}

void ItemModel::setArchiveQuery()
{
    modelQuery = this->modelQueryBase + this->modelQueryArchive + this->modelQuerySortUser;
    Q_EMIT dataChanged();
}

void ItemModel::setComponentQuery()
{
    modelQuery = this->modelQueryBase + this->modelQueryIncludeComponents + this->modelQuerySortUser;
    Q_EMIT dataChanged();
}

void ItemModel::refresh()
{
    this->setQuery(this->modelQuery);
}
