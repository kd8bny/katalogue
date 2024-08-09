#include "item.h"

QVariantList Item::asList() const
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
    list.append(QVariant(this->getUserOrder()));
    list.append(QVariant(this->getItemId()));

    return list;
}
