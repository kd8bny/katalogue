#include "itemModel.h"
#include "database.h"


ItemModel::ItemModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    this->updateModel();
}

ItemModel::~ItemModel()
{

}

QVariant ItemModel::data(const QModelIndex & index, int role) const {

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

void ItemModel::updateModel()
{
    this->setQuery(QString("SELECT id, %1, %2 , %3, %4, %5 FROM %6 WHERE %7 IS NULL").arg(
            NAME, YEAR, MODEL, MAKE, TYPE, TABLE_ITEMS, KEY_ITEM_ID));
}

void ItemModel::setModelComponents(){
    this->setQuery(QString("SELECT id, %1, %2 , %3, %4, %5 FROM %6 WHERE %7 IS NOT NULL").arg(
            NAME, YEAR, MODEL, MAKE, TYPE, TABLE_ITEMS, KEY_ITEM_ID));
}

int ItemModel::getId(int row)
{
    return this->data(this->index(row, 0), rID).toInt();
}

QVariantList ItemModel::getItemData(QString item_id)
{
    QSqlQuery query;
    QVariantList itemData;
    query.prepare(QString(
        "SELECT %1, %2, %3, %4, %5, %6, %7, %8 FROM %9 WHERE id=:item_id").arg(
            NAME, MAKE, MODEL, YEAR, TYPE, GROUP, ARCHIVED, KEY_ITEM_ID,
            TABLE_ITEMS));
    query.bindValue(":item_id", item_id.toInt());
    query.exec();
    query.next();

    for (int i=0; i<query.record().count(); i++)
    {
        itemData.append(query.value(i));
    }

    return itemData;
}

QVariantList ItemModel::getItemTypes()
{
    QSqlQuery query;
    QVariantList itemTypes;
    query.prepare(QString(
        "SELECT DISTINCT %1 FROM %2").arg(TYPE, TABLE_ITEMS));

    query.exec();

    while(query.next())
    {
        itemTypes.append(query.record().value(0));
    }

    return itemTypes;
}

QVariantList ItemModel::getItemParents()
{
    QSqlQuery query;
    QVariantList itemParents;
    query.prepare(QString( 
        "SELECT DISTINCT %1 FROM %2 WHERE %3 IS NULL").arg(NAME, TABLE_ITEMS,
            KEY_ITEM_ID));

    query.exec();
    while(query.next())
    {
        itemParents.append(query.record().value(0));
    }

    return itemParents;
}
