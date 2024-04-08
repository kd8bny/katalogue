#include "eventModel.h"


EventModel::EventModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    QObject::connect(this, SIGNAL(dataChanged()), this, SLOT(refresh()));

    this->refresh();
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
    roles[rDate] = Database::DATE.toUtf8();
    roles[rEvent] = Database::EVENT.toUtf8();
    roles[rCost] = Database::COST.toUtf8();
    roles[rOdometer] = Database::ODOMETER.toUtf8();
    roles[rCategory] = Database::CATEGORY.toUtf8();
    roles[rComment] = Database::COMMENT.toUtf8();
    roles[rItemID] = Database::KEY_ITEM_ID.toUtf8();

    return roles;
}

void EventModel::setItemId(QString itemId)
{
    this->modelQuery = this->modelQueryBase + this->modelQuerySetId.arg(
        Database::KEY_ITEM_ID, itemId);

    this->setQuery(modelQuery);
    Q_EMIT dataChanged();
}

void EventModel::refresh()
{
    this->setQuery(this->modelQuery);
    qDebug()<< this->modelQuery;
}

int EventModel::getId(int row)
{
    return this->data(this->index(row, 0), rID).toInt();
}

Event EventModel::getRecord(int row)
{
    int id = this->data(this->index(row, 0), rID).toInt();
    Event event(id);

    event.setDate(this->data(this->index(row, 1), rDate).toString());
    event.setEvent(this->data(this->index(row, 2), rEvent).toString());
    event.setCost(this->data(this->index(row, 3), rCost).toFloat());
    event.setOdometer(this->data(this->index(row, 4), rOdometer).toFloat());
    event.setCategory(this->data(this->index(row, 5), rCategory).toString());
    event.setComment(this->data(this->index(row, 6), rComment).toString());
    event.setItemId(this->data(this->index(row, 7), rItemID).toInt());

    return event;
}

QVariantList EventModel::getRecordAsList(int row)
{
    Event event = this->getRecord(row);

    return event.asList();
}

bool EventModel::setRecord(int eventIndex, QString date, QString eventName,
    float cost, float odometer, QString category, QString comment, int itemId)
{
    Database db;
    // eventIndex defaults to -1 for new entries.
    Event event(this->getId(eventIndex));

    bool success = false;

    event.setDate(date);
    event.setEvent(eventName);
    event.setCost(cost);
    event.setOdometer(odometer);
    event.setCategory(category);
    event.setComment(comment);
    event.setItemId(itemId);

    qDebug() << event.asList();

    if (eventIndex == -1) {
        success = db.insertEventEntry(event);
        qDebug() << "eventModel | Inserting Entry | " << success;
    } else {
        success = db.updateEventEntry(event);
        qDebug() << "eventModel | Updating Entry | " << success;
    }

    if (success)
        Q_EMIT dataChanged();

    return success;

}

bool EventModel::deleteRecord(int eventId)
{
    Database db;

    bool success = false;

    success = db.deleteEventEntry(eventId);

    if (success)
        Q_EMIT dataChanged();

    return success;
}
