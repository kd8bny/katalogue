#include "itemParentModel.h"


ItemParentModel::ItemParentModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    QObject::connect(this, SIGNAL(dataChanged()), this, SLOT(refresh()));

    this->refresh();
}

ItemParentModel::~ItemParentModel()
{

}

void ItemParentModel::refresh()
{
    this->setQuery(this->modelQuery);
}

int ItemParentModel::getId(int row)
{
    // First select is the combobox values, second is the Id of the record
    // This is set in the modelQuery
    return this->record(row).value(1).toInt();
}
