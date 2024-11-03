#include "noteModel.h"

NoteModel::NoteModel(QObject *parent) : QSqlQueryModel(parent)
{
    QObject::connect(this, &NoteModel::filterItemId, this, &NoteModel::setItemIdQuery);
    QObject::connect(this, &NoteModel::resetFilterItemId, this, &NoteModel::resetItemIdQuery);
    QObject::connect(this, SIGNAL(modelQueryChanged()), this, SLOT(refreshModel()));

    this->refreshModel();
}

// The method for obtaining data from the model
QVariant NoteModel::data(const QModelIndex &index, int role) const
{

    // Define the column number, on the role of number
    int columnId = role - Qt::UserRole - 1;
    // Create the index using a column ID
    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> NoteModel::roleNames() const
{

    QHash<int, QByteArray> roles;
    roles[rID] = "id";
    roles[rTitle] = DatabaseSchema::TITLE.toUtf8();
    roles[rNote] = DatabaseSchema::NOTE_CONTENT.toUtf8();
    roles[rItemID] = DatabaseSchema::KEY_ITEM_ID.toUtf8();

    return roles;
}

int NoteModel::getId(int row) const
{
    return this->data(this->index(row, 0), rID).toInt();
}

void NoteModel::setItemIdQuery(QString itemId)
{
    this->modelQuery = this->modelQueryBase + this->modelQuerySetId.arg(DatabaseSchema::KEY_ITEM_ID, itemId);

    this->setQuery(modelQuery);
    Q_EMIT modelQueryChanged();
}

void NoteModel::resetItemIdQuery()
{
    this->modelQuery = this->modelQueryBase;
    Q_EMIT modelQueryChanged();
}

void NoteModel::refreshModel()
{
    this->setQuery(this->modelQuery);
    qDebug() << this->modelQuery;
}
