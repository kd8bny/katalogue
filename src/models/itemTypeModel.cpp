#include "itemTypeModel.h"


ItemTypeModel::ItemTypeModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    QObject::connect(this, SIGNAL(dataChanged()), this, SLOT(refresh()));

    this->refresh();
}

ItemTypeModel::~ItemTypeModel()
{

}

void ItemTypeModel::refresh()
{
    this->setQuery(this->modelQuery);
}
