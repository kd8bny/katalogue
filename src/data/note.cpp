#include "note.h"

Note::Note(int id_)
{
    id = id_;
}

Note::~Note()
{

}

QVariantList Note::asList()
{
    QVariantList list;

    list.append(this->getId());
    list.append(this->getTitle());
    list.append(this->getNote());
    list.append(this->getItemId());

    return list;
}