#include "attributeCategoryModel.h"
#include "database.h"


AttributeCategoryModel::AttributeCategoryModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    QObject::connect(this, SIGNAL(dataChanged()), this, SLOT(refresh()));

    this->refresh();
}

AttributeCategoryModel::~AttributeCategoryModel()
{

}

void AttributeCategoryModel::refresh()
{
    this->setQuery(this->modelQuery);
}
