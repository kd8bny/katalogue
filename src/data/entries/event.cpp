#include "event.h"

// Event::~Event() = default;

QVariantList Event::asList() const
{
    QVariantList list;

    list.append(QVariant(this->getId()));
    list.append(QVariant(this->getCreatedDate()));
    list.append(QVariant(this->getModifiedDate()));
    list.append(QVariant(this->getDate()));
    list.append(QVariant(this->getEvent()));
    list.append(QVariant(this->getCost()));
    list.append(QVariant(this->getIncrement()));
    list.append(QVariant(this->getCategory()));
    list.append(QVariant(this->getComment()));
    list.append(QVariant(this->getItemId()));

    return list;
}
