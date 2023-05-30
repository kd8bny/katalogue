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
