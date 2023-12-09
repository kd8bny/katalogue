#include "todo.h"

Todo::Todo(int id_)
{
    id = id_;
}

Todo::~Todo()
{

}

QVariantList Todo::asList()
{
    QVariantList list;

    list.append(this->getId());
    list.append(this->getTitle());
    list.append(this->getDescription());
    list.append(this->getDueDate());

    QVariantList asList();
    return list;
}
