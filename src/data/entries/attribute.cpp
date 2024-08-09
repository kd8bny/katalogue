#include "attribute.h"

Attribute::Attribute(int id_)
{
    id = id_;
}

Attribute::~Attribute()
{

}

QVariantList Attribute::asList()
{
    QVariantList list;

    list.append(QVariant(this->getId()));
    list.append(QVariant(this->getCreatedDate()));
    list.append(QVariant(this->getModifiedDate()));
    list.append(QVariant(this->getKey()));
    list.append(QVariant(this->getValue()));
    list.append(QVariant(this->getCategory()));
    list.append(QVariant(this->getItemId()));

    return list;
}
