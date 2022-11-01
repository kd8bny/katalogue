#include "componentModel.h"
#include "database.h"


ComponentModel::ComponentModel(QString itemId, QObject *parent) :
    QSqlQueryModel(parent)
{
    modelQuery = QString("SELECT id, %1, %2, %3, %4, %5, %6 FROM %7").arg(
            NAME, MAKE, MODEL, YEAR, TYPE, ARCHIVED, TABLE_ITEMS);
}

ComponentModel::~ComponentModel()
{

}

QVariant ComponentModel::data(const QModelIndex & index, int role) const {

    // Define the column number, on the role of number
    int columnId = role - Qt::UserRole - 1;
    // Create the index using a column ID
    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> ComponentModel::roleNames() const {

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

void ComponentModel::updateModel(QString itemId){
    this->setQuery(QString("%1 WHERE %2=%3").arg(
            modelQuery, KEY_ITEM_ID, itemId));
}

int ComponentModel::getId(int row)
{
    return this->data(this->index(row, 0), rID).toInt();
}

QVariantList ComponentModel::getRecord(int row)
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
