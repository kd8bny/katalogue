// ref: https://evileg.com/en/post/189/#header_main.cpp
#include "itemModel.h"
#include "database.h"


ItemModel::ItemModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    this->updateModel();
}

ItemModel::~ItemModel()
{

}

// The method for obtaining data from the model
QVariant ItemModel::data(const QModelIndex & index, int role) const {

    // Define the column number, on the role of number
    int columnId = role - Qt::UserRole - 1;
    // Create the index using a column ID
    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> ItemModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[rID] = "id";
    roles[rUID] = TABLE_UUID;
    roles[rNAME] = TABLE_NAME;
    roles[rMAKE] = TABLE_MAKE;
    roles[rMODEL] = TABLE_MODEL;
    roles[rYEAR] = TABLE_YEAR;
    roles[rGroup] = TABLE_GROUP;
    roles[rCategory] = TABLE_CATEGORY;
    roles[rArchived] = TABLE_ARCHIVED;

    return roles;
}

// The method updates the tables in the data model representation
void ItemModel::updateModel()
{
    // The update is performed SQL-queries to the database
    this->setQuery("SELECT id, " TABLE_UUID ", " TABLE_NAME ", " TABLE_YEAR ", "
                   TABLE_MODEL ", " TABLE_MAKE " FROM " TABLE_ITEMS);
}

//Getting the id of the row in the data view model
int ItemModel::getId(int row)
{
    return this->data(this->index(row, 0), rID).toInt();
}
