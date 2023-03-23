#include "database.h"
#include "eventModel.h"


EventModel::EventModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    modelQuery = QString("SELECT id, %1, %2, %3, %4, %5, %6, %7 FROM %8").arg(
        DATE, EVENT, COST, ODOMETER, TYPE, CATEGORY, COMMENT, TABLE_EVENTS);

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
    roles[rDate] = DATE;
    roles[rEvent] = EVENT;
    roles[rCost] = COST;
    roles[rOdometer] = ODOMETER;
    roles[rType] = TYPE;
    roles[rCategory] = CATEGORY;
    roles[rComment] = COMMENT;
    roles[rItemID] = KEY_ITEM_ID;

    return roles;
}

// The method updates the tables in the data model representation
void EventModel::updateModel()
{
    this->setQuery(modelQuery);
}

void EventModel::setItemId(QString itemId)
{
    QString modelQueryID = QString("%1 WHERE %2=%3").arg(modelQuery, KEY_ITEM_ID, itemId);
    this->setQuery(modelQueryID);
}

int EventModel::getId(int row)
{
    return this->data(this->index(row, 0), rID).toInt();
}

QVariantList EventModel::getRecord(int row)
{
    QVariantList recordData;

    recordData.append(this->data(this->index(row, 0), rID).toInt());
    recordData.append(this->data(this->index(row, 1), rDate));
    recordData.append(this->data(this->index(row, 2), rEvent));
    recordData.append(this->data(this->index(row, 3), rCost));
    recordData.append(this->data(this->index(row, 4), rOdometer));
    recordData.append(this->data(this->index(row, 5), rType));
    recordData.append(this->data(this->index(row, 6), rCategory));
    recordData.append(this->data(this->index(row, 7), rComment));

    qDebug() << recordData;

    return recordData;
}
