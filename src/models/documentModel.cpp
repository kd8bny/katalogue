#include "documentModel.h"

DocumentModel::DocumentModel(QObject *parent) : QSqlQueryModel(parent)
{
    QObject::connect(this, &DocumentModel::filterItemId, this, &DocumentModel::setItemIdQuery);
    QObject::connect(this, &DocumentModel::resetFilterItemId, this, &DocumentModel::resetItemIdQuery);
    QObject::connect(this, SIGNAL(modelQueryChanged()), this, SLOT(refreshModel()));

    this->refreshModel();
}

// The method for obtaining data from the model
QVariant DocumentModel::data(const QModelIndex &index, int role) const
{

    // Define the column number, on the role of number
    int columnId = role - Qt::UserRole - 1;
    // Create the index using a column ID
    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> DocumentModel::roleNames() const
{

    QHash<int, QByteArray> roles;
    roles[rID] = DatabaseSchema::ID.toUtf8();
    roles[rName] = DatabaseSchema::NAME.toUtf8();
    roles[rFileName] = DatabaseSchema::FILE_NAME.toUtf8();

    return roles;
}

int DocumentModel::getId(int row) const
{
    return this->data(this->index(row, 0), rID).toInt();
}

void DocumentModel::setItemIdQuery(QString itemId)
{
    this->modelQuery = this->modelQueryBase + this->modelQuerySetId.arg(DatabaseSchema::KEY_ITEM_ID, itemId);

    this->setQuery(modelQuery);
    Q_EMIT modelQueryChanged();
}

void DocumentModel::resetItemIdQuery()
{
    this->modelQuery = this->modelQueryBase;
    Q_EMIT modelQueryChanged();
}

void DocumentModel::refreshModel()
{
    this->setQuery(this->modelQuery);
    qDebug() << this->modelQuery;
}
