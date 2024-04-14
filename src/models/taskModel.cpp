#include "taskModel.h"


TaskModel::TaskModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    QObject::connect(this, SIGNAL(dataChanged()), this, SLOT(refresh()));

    this->refresh();
}

TaskModel::~TaskModel()
{

}

// The method for obtaining data from the model
QVariant TaskModel::data(const QModelIndex & index, int role) const {

    // Define the column number, on the role of number
    int columnId = role - Qt::UserRole - 1;
    // Create the index using a column ID
    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> TaskModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[rID] = "id";
    roles[rDueDate] = Database::DUE_DATE.toUtf8();
    roles[rStatus] = Database::STATUS.toUtf8();
    roles[rTitle] = Database::TITLE.toUtf8();
    roles[rDescription] = Database::DESCRIPTION.toUtf8();
    roles[rItemID] = Database::KEY_ITEM_ID.toUtf8();

    return roles;
}

void TaskModel::setItemId(QString itemId)
{
    this->modelQuery = this->modelQueryBase + this->modelQuerySetId.arg(
        Database::KEY_ITEM_ID, itemId);

    this->setQuery(modelQuery);
    Q_EMIT dataChanged();
}

void TaskModel::refresh()
{
    this->setQuery(this->modelQueryBase);
    qDebug()<< this->modelQueryBase;
}

int TaskModel::getId(int row)
{
    return this->data(this->index(row, 0), rID).toInt();
}

Task TaskModel::getRecord(int row)
{
    int id = this->data(this->index(row, 0), rID).toInt();
    Task task(id);
    task.setDueDate(this->data(this->index(row, 1), rTitle).toString());
    task.setStatus(this->data(this->index(row, 2), rTitle).toString());
    task.setTitle(this->data(this->index(row, 3), rTitle).toString());
    task.setDescription(this->data(this->index(row, 4), rTitle).toString());
    task.setItemId(this->data(this->index(row, 5), rItemID).toInt());

    return task;
}

QVariantList TaskModel::getRecordAsList(int row)
{
    Task task = this->getRecord(row);

    return task.asList();
}

bool TaskModel::setRecord(int taskIndex, QString dueDate, QString status, QString title, QString description, int itemId)
{
    Database db;
    // taskIndex defaults to -1 for new entries.
    Task task(this->getId(taskIndex));

    bool success = false;

    task.setDueDate(dueDate);
    task.setStatus(status);
    task.setTitle(title);
    task.setDescription(description);
    task.setItemId(itemId);

    qDebug() << task.asList();

    if (taskIndex == -1) {
        success = db.insertTaskEntry(task);
        qDebug() << "taskModel | Inserting Entry | " << success;
    } else {
        success = db.updateTaskEntry(task);
        qDebug() << "taskModel | Updating Entry | " << success;
    }

    if (success)
        Q_EMIT dataChanged();

    return success;

}

bool TaskModel::deleteRecord(int taskId)
{
    Database db;

    bool success = false;

    success = db.deleteTaskEntry(taskId);

    if (success)
        Q_EMIT dataChanged();

    return success;
}
