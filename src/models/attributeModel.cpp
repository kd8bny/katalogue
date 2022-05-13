#include "attributeModel.h"
#include "database.h"


AttributeModel::AttributeModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    this->updateModel();
}

AttributeModel::~AttributeModel()
{

}

// The method for obtaining data from the model
QVariant AttributeModel::data(const QModelIndex & index, int role) const {

    // Define the column number, on the role of number
    int columnId = role - Qt::UserRole - 1;
    // Create the index using a column ID
    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> AttributeModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[rID] = "id";
    roles[rCategory] = TABLE_CATEGORY;
    roles[rKey] = TABLE_KEY;
    roles[rValue] = TABLE_VALUE;

    return roles;
}

// The method updates the tables in the data model representation
void AttributeModel::updateModel()
{
    // The update is performed SQL-queries to the database
    this->setQuery("SELECT id, " TABLE_CATEGORY ", " TABLE_KEY ", "
                   TABLE_VALUE " FROM " TABLE_ATTRIBUTES);
}

//Getting the id of the row in the data view model
int AttributeModel::getId(int row)
{
    return this->data(this->index(row, 0), rID).toInt();
}
