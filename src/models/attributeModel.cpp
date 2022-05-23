#include "attributeModel.h"
#include "database.h"

AttributeModel::AttributeModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    modelQuery = QString("SELECT id, %1,%2,%3 FROM %4 ").arg(
        TABLE_LABEL, TABLE_KEY, TABLE_VALUE, TABLE_ATTRIBUTES);

    this->setModelQuery();
}

AttributeModel::~AttributeModel()
{

}

// The method for obtaining data from the model
QVariant AttributeModel::data(const QModelIndex & index, int role) const {

    // Define the column number, on the role of number
    int columnId = role - Qt::UserRole - 1;
    // Create the index using a column ID
    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> AttributeModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[rID] = "id";
    roles[rLabel] = TABLE_LABEL;
    roles[rKey] = TABLE_KEY;
    roles[rValue] = TABLE_VALUE;
    roles[rItemID] = TABLE_ITEM_ID;

    return roles;
}

void AttributeModel::setModelQuery()
{
    this->setQuery(modelQuery);
}

void AttributeModel::setItemID(QString item_id)
{
    QString modelQueryID = QString("%1 WHERE %2=%3").arg(modelQuery, TABLE_ITEM_ID, item_id);
    this->setQuery(modelQueryID);
}

//Getting the id of the row in the data view model
int AttributeModel::getId(int row)
{
    return this->data(this->index(row, 0), rID).toInt();
}
