#include "database.h"
#include "eventModel.h"


EventModel::EventModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    modelQuery = QString("SELECT id, %1,%2,%3, %4, %5, %6 FROM %7").arg(
        DATE, EVENT, COST, TYPE, CATEGORY, COMMENT, TABLE_EVENTS);

    this->setModelQuery();
}

EventModel::~EventModel()
{

}

// The method for obtaining data from the model
QVariant EventModel::data(const QModelIndex & index, int role) const {

    // Define the column number, on the role of number
    int columnId = role - Qt::UserRole - 1;
    // Create the index using a column ID
    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> EventModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[rID] = "id";
    roles[rDate] = DATE;
    roles[rEvent] = EVENT;
    roles[rCost] = COST;
    roles[rType] = TYPE;
    roles[rCategory] = CATEGORY;
    roles[rComment] = COMMENT;
    roles[rItemID] = KEY_ITEM_ID;

    return roles;
}

// The method updates the tables in the data model representation
void EventModel::setModelQuery()
{
    this->setQuery(modelQuery);
}

void EventModel::setItemID(QString item_id)
{
    QString modelQueryID = QString("%1 WHERE %2=%3").arg(modelQuery, KEY_ITEM_ID, item_id);
    this->setQuery(modelQueryID);
}

//Getting the id of the row in the data view model
int EventModel::getId(int row)
{
    return this->data(this->index(row, 0), rID).toInt();
}
