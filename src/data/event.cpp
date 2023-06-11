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

    list.append(this->getId());
    list.append(this->getItemId());
    list.append(this->getDate());
    list.append(this->getCost());
    list.append(this->getOdometer());
    list.append(this->getCategory());
    list.append(this->getType());
    list.append(this->getComment());

    return list;
}
