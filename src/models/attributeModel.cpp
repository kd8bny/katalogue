#include "attributeModel.h"

AttributeModel::AttributeModel(QObject *parent) : QSqlQueryModel(parent)
{
    // QObject::connect(this, SIGNAL(dataChanged()), this, SLOT(refresh()));

    // this->refresh();
}

AttributeModel::~AttributeModel()
{
}

// The method for obtaining data from the model
QVariant AttributeModel::data(const QModelIndex &index, int role) const
{

    // Define the column number, on the role of number
    int columnId = role - Qt::UserRole - 1;
    // Create the index using a column ID
    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> AttributeModel::roleNames() const
{

    QHash<int, QByteArray> roles;
    roles[rID] = "id";
    roles[rKey] = DatabaseSchema::KEY.toUtf8();
    roles[rValue] = DatabaseSchema::VALUE.toUtf8();
    roles[rCategory] = DatabaseSchema::CATEGORY.toUtf8();
    roles[rItemID] = DatabaseSchema::KEY_ITEM_ID.toUtf8();

    return roles;
}

void AttributeModel::setItemId(QString itemId)
{
    this->modelQuery = this->modelQueryBase + this->modelQuerySetId.arg(DatabaseSchema::KEY_ITEM_ID, itemId);

    this->setQuery(modelQuery);
    Q_EMIT dataChanged();
}

void AttributeModel::refresh()
{
    this->setQuery(this->modelQuery);
    qDebug() << this->modelQuery;
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
    AttributeDatabase db;
    // eventIndex defaults to -1 for new entries.
    Attribute attribute(this->getId(attributeIndex));

    bool success = false;

    attribute.setKey(key);
    attribute.setValue(value);
    attribute.setCategory(category);
    attribute.setItemId(itemId);

    qDebug() << attribute.asList();

    if (attributeIndex == -1)
    {
        success = db.insertEntry(attribute);
        qDebug() << "attributeModel | Inserting Entry | " << success;
    }
    else
    {
        success = db.updateEntry(attribute);
        qDebug() << "attributeModel | Updating Entry | " << success;
    }

    if (success)
        Q_EMIT dataChanged();

    return success;
}

bool AttributeModel::deleteRecord(int eventId)
{
    AttributeDatabase db;

    bool success = false;

    success = db.deleteEntryById(eventId);

    if (success)
        Q_EMIT dataChanged();

    return success;
}
