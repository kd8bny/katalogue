#include "taskModel.h"

TaskModel::TaskModel(QObject *parent) : QSqlQueryModel(parent)
{
    QObject::connect(this, &TaskModel::filterItemId, this, &TaskModel::setItemIdQuery);
    QObject::connect(this, &TaskModel::resetFilterItemId, this, &TaskModel::resetItemIdQuery);
    QObject::connect(this, &TaskModel::modelQueryChanged, this, &TaskModel::onModelQueryChanged);

    this->onModelQueryChanged();
}

// The method for obtaining data from the model
QVariant TaskModel::data(const QModelIndex &index, int role) const
{

    // Define the column number, on the role of number
    int columnId = role - Qt::UserRole - 1;
    // Create the index using a column ID
    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> TaskModel::roleNames() const
{

    QHash<int, QByteArray> roles;
    roles[rID] = "id";
    roles[rDueDate] = DatabaseSchema::DUE_DATE.toUtf8();
    roles[rStatus] = DatabaseSchema::STATUS.toUtf8();
    roles[rTitle] = DatabaseSchema::TITLE.toUtf8();
    roles[rDescription] = DatabaseSchema::DESCRIPTION.toUtf8();
    roles[rItemID] = DatabaseSchema::KEY_ITEM_ID.toUtf8();

    return roles;
}

int TaskModel::getId(int row) const
{
    return this->data(this->index(row, 0), rID).toInt();
}

void TaskModel::setItemIdQuery(QString itemId)
{
    this->modelQuery = this->modelQueryBase + this->modelQuerySetId.arg(
                                                  DatabaseSchema::KEY_ITEM_ID, itemId);

    this->setQuery(modelQuery);
    Q_EMIT modelQueryChanged();
}

void TaskModel::resetItemIdQuery()
{
    this->modelQuery = this->modelQueryBase;
    Q_EMIT modelQueryChanged();
}

void TaskModel::onModelQueryChanged()
{
    this->setQuery(this->modelQuery);
    qDebug() << this->modelQuery;
}
