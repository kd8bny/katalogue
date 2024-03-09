#include "item.h"

Item::Item(int id_)
{
    id = id_;
}

Item::~Item()
{

}

QVariantList Item::asList()
{
    QVariantList list;

    list.append(this->getId());
    list.append(this->getCreatedDate());
    list.append(this->getModifiedDate());
    list.append(this->getName());
    list.append(this->getMake());
    list.append(this->getModel());
    list.append(this->getYear());
    list.append(this->getType());
    list.append(this->getArchived());
    list.append(this->getParent());

    return list;
}
