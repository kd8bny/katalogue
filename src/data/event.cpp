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
    list.append(this->getDate());
    list.append(this->getEvent());
    list.append(this->getCost());
    list.append(this->getOdometer());
    list.append(this->getCategory());
    list.append(this->getComment());
    list.append(this->getItemId());

    return list;
}
