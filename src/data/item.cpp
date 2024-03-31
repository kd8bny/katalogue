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

    list.append(QVariant(this->getId()));
    list.append(QVariant(this->getCreatedDate()));
    list.append(QVariant(this->getModifiedDate()));
    list.append(QVariant(this->getName()));
    list.append(QVariant(this->getMake()));
    list.append(QVariant(this->getModel()));
    list.append(QVariant(this->getYear()));
    list.append(QVariant(this->getType()));
    list.append(QVariant(this->getArchived()));
    list.append(QVariant(this->getParent()));

    return list;
}
