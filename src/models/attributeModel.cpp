#include "attributeModel.h"

AttributeModel::AttributeModel(QObject *parent) : QSqlQueryModel(parent)
{
    QObject::connect(this, &AttributeModel::filterItemId, this, &AttributeModel::setItemIdQuery);
}

QVariant AttributeModel::data(const QModelIndex &index, int role) const
{

    // Define the column number, on the role of number
    int columnId = role - Qt::UserRole - 1;
    // Create the index using a column ID
    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> AttributeModel::roleNames() const
{

    QHash<int, QByteArray> roles;
    roles[rID] = DatabaseSchema::ID.toUtf8();
    roles[rKey] = DatabaseSchema::KEY.toUtf8();
    roles[rValue] = DatabaseSchema::VALUE.toUtf8();
    roles[rCategory] = DatabaseSchema::CATEGORY.toUtf8();
    roles[rItemID] = DatabaseSchema::KEY_ITEM_ID.toUtf8();

    return roles;
}

int AttributeModel::getId(int row) const
{
    return this->data(this->index(row, 0), rID).toInt();
}

void AttributeModel::setItemIdQuery(QString itemId)
{
    this->modelQuery = this->modelQueryBase + this->modelQuerySetId.arg(DatabaseSchema::KEY_ITEM_ID, itemId);
    this->setQuery(modelQuery);
    Q_EMIT dataChanged();
}

void AttributeModel::refresh()
{
    this->setQuery(this->modelQuery);
    qDebug() << this->modelQuery;
}
