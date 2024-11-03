#include "eventModel.h"

EventModel::EventModel(QObject *parent) : QSqlQueryModel(parent)
{
    QObject::connect(this, &EventModel::filterItemId, this, &EventModel::setItemIdQuery);
    QObject::connect(this, &EventModel::modelQueryChanged, this, &EventModel::refreshModel);

    this->refreshModel();
}

// The method for obtaining data from the model
QVariant EventModel::data(const QModelIndex &index, int role) const
{

    // Define the column number, on the role of number
    int columnId = role - Qt::UserRole - 1;
    // Create the index using a column ID
    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> EventModel::roleNames() const
{

    QHash<int, QByteArray> roles;
    roles[rID] = "id";
    roles[rDate] = DatabaseSchema::DATE.toUtf8();
    roles[rEvent] = DatabaseSchema::EVENT.toUtf8();
    roles[rCost] = DatabaseSchema::COST.toUtf8();
    roles[rIncrement] = DatabaseSchema::INCREMENT.toUtf8();
    roles[rCategory] = DatabaseSchema::CATEGORY.toUtf8();
    roles[rComment] = DatabaseSchema::COMMENT.toUtf8();
    roles[rItemID] = DatabaseSchema::KEY_ITEM_ID.toUtf8();

    return roles;
}

int EventModel::getId(int row) const
{
    return this->data(this->index(row, 0), rID).toInt();
}

void EventModel::setItemIdQuery(QString itemId)
{
    this->modelQuery = this->modelQueryBase + this->modelQuerySetId.arg(DatabaseSchema::KEY_ITEM_ID, itemId);
    this->setQuery(modelQuery);
    Q_EMIT modelQueryChanged();
}

void EventModel::refreshModel()
{
    this->setQuery(this->modelQuery);
    qDebug() << this->modelQuery;
}
