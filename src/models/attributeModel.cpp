#include "attributeModel.h"
#include "database.h"

AttributeModel::AttributeModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    modelQuery = QString("SELECT id, %1, %2, %3, %4 FROM %5 ").arg(
        KEY, VALUE, CATEGORY, KEY_ITEM_ID, TABLE_ATTRIBUTES);

    this->updateModel();
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
    roles[rKey] = KEY;
    roles[rValue] = VALUE;
    roles[rCategory] = CATEGORY;
    roles[rItemID] = KEY_ITEM_ID;

    return roles;
}

void AttributeModel::updateModel()
{
    this->setQuery(modelQuery);
}

void AttributeModel::setItemId(QString item_id)
{
    QString modelQueryID = QString("%1 WHERE %2=%3").arg(modelQuery, KEY_ITEM_ID, item_id);
    this->setQuery(modelQueryID);
    qDebug() << this->lastError();
}

int AttributeModel::getId(int row)
{
    return this->data(this->index(row, 0), rID).toInt();
}

QVariantList AttributeModel::getRecord(int row)
{
    QVariantList recordData;

    recordData.append(this->data(this->index(row, 0), rID).toInt());
    recordData.append(this->data(this->index(row, 1), rKey));
    recordData.append(this->data(this->index(row, 2), rValue));
    recordData.append(this->data(this->index(row, 3), rCategory));

    return recordData;
}
