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

    list.append(QVariant(this->getId()));
    list.append(QVariant(this->getCreatedDate()));
    list.append(QVariant(this->getModifiedDate()));
    list.append(QVariant(this->getTitle()));
    list.append(QVariant(this->getNoteContent()));
    list.append(QVariant(this->getItemId()));

    return list;
}
