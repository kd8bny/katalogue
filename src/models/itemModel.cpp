#include "itemModel.h"


ItemModel::ItemModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    QObject::connect(this, SIGNAL(filterItem()), this, SLOT(setItemQuery()));
    QObject::connect(this, SIGNAL(filterArchive()), this, SLOT(setArchiveQuery()));
    QObject::connect(this, SIGNAL(filterComponent()), this, SLOT(setComponentQuery()));
    QObject::connect(this, SIGNAL(dataChanged()), this, SLOT(refresh()));

    emit setItemQuery();
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
    roles[rNAME] = NAME;
    roles[rMAKE] = MAKE;
    roles[rMODEL] = MODEL;
    roles[rYEAR] = YEAR;
    roles[rTYPE] = TYPE;
    roles[rARCHIVED] = ARCHIVED;
    roles[rPARENT] = KEY_ITEM_ID;

    return roles;
}

void ItemModel::setItemQuery()
{
    modelQuery = this->modelQueryBase + this->modelQueryItem;
    emit dataChanged();
}

void ItemModel::setArchiveQuery()
{
    modelQuery = this->modelQueryBase + this->modelQueryArchive;
    emit dataChanged();
}

void ItemModel::setComponentQuery()
{
    modelQuery = this->modelQueryBase + this->modelQueryComponent;
    emit dataChanged();
}

void ItemModel::refresh()
{
    this->setQuery(this->modelQuery);
}

int ItemModel::getId(int row)
{
    return this->data(this->index(row, 0), rID).toInt();
}

Item ItemModel::getRecord(int row)
{
    int id = this->data(this->index(row, 0), rID).toInt();
    Item item(id);

    item.setName(this->data(this->index(row, 1), rNAME).toString());
    item.setMake(this->data(this->index(row, 2), rMAKE).toString());
    item.setModel(this->data(this->index(row, 3), rMODEL).toString());
    item.setYear(this->data(this->index(row, 4), rYEAR).toInt());
    item.setType(this->data(this->index(row, 5), rTYPE).toString());
    item.setArchived(this->data(this->index(row, 6), rARCHIVED).toInt());
    item.setParent(this->data(this->index(row, 7), rPARENT).toInt());

    return item;
}

QVariantList ItemModel::getRecordAsList(int row)
{
    Item item = this->getRecord(row);

    return item.asList();
}

bool ItemModel::setRecord(int itemIndex, QString name, QString make,
    QString model, int year, QString type, int archived, int parent)
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
        emit dataChanged();

    return success;

}

bool ItemModel::deleteRecord(int itemId)
{
    Database db;

    bool success = false;

    success = db.deleteItemEntry(itemId);

    if (success)
        emit dataChanged();

    return success;
}
