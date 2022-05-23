#include "eventModel.h"
#include "database.h"


EventModel::EventModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    this->updateModel();
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
    roles[rDate] = TABLE_DATE;
    roles[rTask] = TABLE_TASK;
    roles[rCost] = TABLE_COST;
    roles[rType] = TABLE_TYPE;
    roles[rCategory] = TABLE_CATEGORY;
    roles[rComment] = TABLE_COMMENT;
    roles[rItemID] = TABLE_ITEM_ID;

    return roles;
}

// The method updates the tables in the data model representation
void EventModel::updateModel()
{
    // The update is performed SQL-queries to the database
    this->setQuery("SELECT id, " TABLE_CATEGORY ", " TABLE_DATE ", "
                   TABLE_TASK " FROM " TABLE_EVENTS);
}

//Getting the id of the row in the data view model
int EventModel::getId(int row)
{
    return this->data(this->index(row, 0), rID).toInt();
}
