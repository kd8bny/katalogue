#include "eventCategoryModel.h"


EventCategoryModel::EventCategoryModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    QObject::connect(this, SIGNAL(dataChanged()), this, SLOT(refresh()));

    this->refresh();
}

EventCategoryModel::~EventCategoryModel()
{

}

void EventCategoryModel::refresh()
{
    this->setQuery(this->modelQuery);
}
