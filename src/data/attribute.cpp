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

    list.append(this->getId());
    list.append(this->getCreatedDate());
    list.append(this->getModifiedDate());
    list.append(this->getKey());
    list.append(this->getValue());
    list.append(this->getCategory());
    list.append(this->getItemId());

    return list;
}
