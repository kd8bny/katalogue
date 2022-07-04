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
    roles[rGroup] = GROUP;
    roles[rCategory] = CATEGORY;
    roles[rArchived] = ARCHIVED;

    return roles;
}

void ItemModel::updateModel()
{
    this->setQuery(QString("SELECT id, %1, %2 , %3, %4 FROM %5").arg(
            NAME, YEAR, MODEL, MAKE, TABLE_ITEMS));
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
        "SELECT %1, %2, %3, %4, %5, %6, %7 FROM %8 WHERE id=:item_id").arg(
            NAME, MAKE, MODEL, YEAR, CATEGORY, GROUP, ARCHIVED, TABLE_ITEMS));
    query.bindValue(":item_id", item_id.toInt());
    query.exec();
    qDebug() << query.next();

    return itemData;
}
