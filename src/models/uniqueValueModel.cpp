#include "uniqueValueModel.h"

UniqueValueModel::UniqueValueModel(QObject *parent) : QSqlQueryModel(parent)
{

    QObject::connect(this, &UniqueValueModel::modelQueryChanged, this, &UniqueValueModel::refreshModel);

    this->refreshModel();
}

void UniqueValueModel::setModelQuery(QString modelQuery)
{
    this->modelQuery = modelQuery;
    Q_EMIT modelQueryChanged();
}

void UniqueValueModel::refreshModel()
{
    this->setQuery(this->modelQuery);
}
