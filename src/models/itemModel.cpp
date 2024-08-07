#include "itemModel.h"


ItemModel::ItemModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    QObject::connect(this, &ItemModel::filterItem, this, &ItemModel::setItemQuery);
    QObject::connect(this, &ItemModel::filterArchive, this, &ItemModel::setArchiveQuery);
    QObject::connect(this, &ItemModel::filterComponent, this, &ItemModel::setComponentQuery);
    QObject::connect(this, &ItemModel::dataChanged, this, &ItemModel::refresh);

    Q_EMIT setItemQuery();
    this->refresh();
}

ItemModel::~ItemModel()
{

}

QVariant ItemModel::data(const QModelIndex &index, int role) const {

    // Define the column number, on the role of number
    int columnId = role - Qt::UserRole - 1;
    // Create the index using a column ID
    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> ItemModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[rID] = "id";
    roles[rNAME] = Database::NAME.toUtf8();
    roles[rMAKE] = Database::MAKE.toUtf8();
    roles[rMODEL] = Database::MODEL.toUtf8();
    roles[rYEAR] = Database::YEAR.toUtf8();
    roles[rTYPE] = Database::TYPE.toUtf8();
    roles[rARCHIVED] = Database::ARCHIVED.toUtf8();
    roles[rUSER_ORDER] = Database::USER_ORDER.toUtf8();
    roles[rPARENT] = Database::KEY_ITEM_ID.toUtf8();

    return roles;
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

void ItemModel::setItemPosition(const int index, const int direction)
{
    Database db;

    for (int i=0; i < this->rowCount(); i++)
    {
        int id = this->data(this->index(index, 0), rID).toInt();
        if (direction == 1) {
            // Move down in list by increasing value
            db.updateItemUserOrder(id, index + 1);

            //Move affected following Item
            int pid = this->data(this->index(index + 1, 0), rID).toInt();
            db.updateItemUserOrder(pid, index - 1);

        } else if (direction == -1) {
            // Move up in list by increasing value
            db.updateItemUserOrder(id, index);

            //Move affected preceding Item
            int pid = this->data(this->index(index - 1, 0), rID).toInt();
            db.updateItemUserOrder(pid, index + 1);
        }
    }

    Q_EMIT dataChanged();
}

void ItemModel::refresh()
{
    this->setQuery(this->modelQuery);
}

int ItemModel::getId(int row)
{
    qDebug() << "getId " << this->data(this->index(row, 0), rID).toInt();
    return this->data(this->index(row, 0), rID).toInt();
}

Item ItemModel::getRecord(int row)
{
    int id = this->data(this->index(row, 0), rID).toInt();
    qDebug() << "getRecord " << id;
    Item item(id);

    item.setName(this->data(this->index(row, 1), rNAME).toString());
    item.setMake(this->data(this->index(row, 2), rMAKE).toString());
    item.setModel(this->data(this->index(row, 3), rMODEL).toString());
    item.setYear(this->data(this->index(row, 4), rYEAR).toInt());
    item.setType(this->data(this->index(row, 5), rTYPE).toString());
    item.setArchived(this->data(this->index(row, 6), rARCHIVED).toInt());
    item.setUserOrder(this->data(this->index(row, 7), rUSER_ORDER).toInt());
    item.setParent(this->data(this->index(row, 8), rPARENT).toInt());

    return item;
}

QVariantList ItemModel::getRecordAsList(int row)
{
    Item item = this->getRecord(row);

    return item.asList();
}

bool ItemModel::setRecord(
    int itemIndex, QString name, QString make, QString model, int year, QString type, int archived, int parent)
{
    Database db;
    // itemIndex defaults to -1 for new entries.
    Item item(this->getId(itemIndex));

    bool success = false;

    item.setName(name);
    item.setMake(make);
    item.setModel(model);
    item.setYear(year);
    item.setType(type);
    item.setArchived(archived);
    item.setParent(parent);

    if (itemIndex == -1) {
        success = db.insertItemEntry(item);
        qDebug() << "itemModel | Inserting Entry | " << success;
    } else {
        success = db.updateItemEntry(item);
        qDebug() << "itemModel | Updating Entry | " << success;
    }

    if (success)
        Q_EMIT dataChanged();

    return success;
}

bool ItemModel::deleteRecord(int itemId)
{
    Database db;

    bool success = false;

    success = db.deleteItemEntry(itemId);

    if (success)
        Q_EMIT dataChanged();

    return success;
}
