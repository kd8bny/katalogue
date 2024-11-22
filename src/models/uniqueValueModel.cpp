#include "uniqueValueModel.h"

UniqueValueModel::UniqueValueModel(QObject *parent) : QSqlQueryModel(parent)
{

    QObject::connect(this, &UniqueValueModel::modelQueryChanged, this, &UniqueValueModel::onModelQueryChanged);
}

void UniqueValueModel::setModelQuery(QString modelQuery)
{
    this->modelQuery = modelQuery;
    Q_EMIT modelQueryChanged();
}

int UniqueValueModel::getId(int row)
{
    // First select is the combobox values, second is the Id of the record
    // This is set in the modelQuery
    return this->record(row).value(1).toInt();
}

void UniqueValueModel::onModelQueryChanged()
{
    this->setQuery(this->modelQuery);
}
