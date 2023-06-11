#include "attributeModel.h"


AttributeModel::AttributeModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    QObject::connect(this, SIGNAL(dataChanged()), this, SLOT(refresh()));

    this->refresh();
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
    roles[rKey] = KEY;
    roles[rValue] = VALUE;
    roles[rCategory] = CATEGORY;
    roles[rItemID] = KEY_ITEM_ID;

    return roles;
}

void AttributeModel::setItemId(QString itemId)
{
    this->modelQuery = this->modelQueryBase + this->modelQuerySetId.arg(
        KEY_ITEM_ID, itemId);

    this->setQuery(modelQuery);
    emit dataChanged();
}

void AttributeModel::refresh()
{
    this->setQuery(this->modelQuery);
    qDebug()<< this->modelQuery;
}

int AttributeModel::getId(int row)
{
    return this->data(this->index(row, 0), rID).toInt();
}

Attribute AttributeModel::getRecord(int row)
{
    int id = this->data(this->index(row, 0), rID).toInt();
    Attribute attribute(id);

    attribute.setKey(this->data(this->index(row, 1), rKey).toString());
    attribute.setValue(this->data(this->index(row, 2), rValue).toString());
    attribute.setCategory(this->data(this->index(row, 3), rCategory).toString());
    attribute.setItemId(this->data(this->index(row, 4), rItemID).toInt());

    return attribute;
}

QVariantList AttributeModel::getRecordAsList(int row)
{
    Attribute attribute = this->getRecord(row);

    return attribute.asList();
}

bool AttributeModel::setRecord(int attributeIndex, QString key, QString value,
    QString category, int itemId)
{
    Database db;
    // eventIndex defaults to -1 for new entries.
    Attribute attribute(this->getId(attributeIndex));

    bool success = false;

    attribute.setKey(key);
    attribute.setValue(value);
    attribute.setCategory(category);
    attribute.setItemId(itemId);

    qDebug() << attribute.asList();

    if (attributeIndex == -1) {
        success = db.insertAttributeEntry(attribute);
        qDebug() << "attributeModel | Inserting Entry | " << success;
    } else {
        success = db.updateAttributeEntry(attribute);
        qDebug() << "attributeModel | Updating Entry | " << success;
    }

    if (success)
        emit dataChanged();

    return success;

}

bool AttributeModel::deleteRecord(int eventId)
{
    Database db;

    bool success = false;

    success = db.deleteAttributeEntry(eventId);

    if (success)
        emit dataChanged();

    return success;
}
