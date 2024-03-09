#include "task.h"

Task::Task(int id_)
{
    id = id_;
}

Task::~Task()
{

}

QVariantList Task::asList()
{
    QVariantList list;

    list.append(this->getId());
    list.append(this->getCreatedDate());
    list.append(this->getModifiedDate());
    list.append(this->getTitle());
    list.append(this->getDescription());
    list.append(this->getDueDate());
    list.append(this->getItemId());

    return list;
}
