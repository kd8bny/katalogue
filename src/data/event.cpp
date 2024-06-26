#include "event.h"

Event::Event(int id_)
{
    id = id_;
}

Event::~Event()
{

}

QVariantList Event::asList()
{
    QVariantList list;

    list.append(QVariant(this->getId()));
    list.append(QVariant(this->getCreatedDate()));
    list.append(QVariant(this->getModifiedDate()));
    list.append(QVariant(this->getDate()));
    list.append(QVariant(this->getEvent()));
    list.append(QVariant(this->getCost()));
    list.append(QVariant(this->getOdometer()));
    list.append(QVariant(this->getCategory()));
    list.append(QVariant(this->getComment()));
    list.append(QVariant(this->getItemId()));

    return list;
}
