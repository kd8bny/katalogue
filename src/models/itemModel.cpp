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
    this->setQuery(QString("SELECT id, %1, %2, %3, %4, %5, %6 FROM %7 WHERE %8 IS NULL").arg(
            NAME, MAKE, MODEL, YEAR, TYPE, ARCHIVED, TABLE_ITEMS, KEY_ITEM_ID));
}

void ItemModel::setModelComponents(QString item_id){
    this->setQuery(QString("SELECT id, %1, %2, %3, %4, %5, %6 FROM %7 WHERE %8=%9").arg(
            NAME, MAKE, MODEL, YEAR, TYPE, ARCHIVED, TABLE_ITEMS, KEY_ITEM_ID, item_id));
}

int ItemModel::getId(int row)
{
    return this->data(this->index(row, 0), rID).toInt();
}

QVariantList ItemModel::getRecord(int row)
{
    QVariantList recordData;

    recordData.append(this->data(this->index(row, 0), rID).toInt());
    recordData.append(this->data(this->index(row, 1), rNAME));
    recordData.append(this->data(this->index(row, 2), rMAKE));
    recordData.append(this->data(this->index(row, 3), rMODEL));
    recordData.append(this->data(this->index(row, 4), rYEAR));
    recordData.append(this->data(this->index(row, 5), rTYPE));
    recordData.append(this->data(this->index(row, 6), rARCHIVED).toInt());
    recordData.append(this->data(this->index(row, 7), rPARENT).toInt());

    qDebug() << row;
    qDebug() << recordData;

    return recordData;
}
