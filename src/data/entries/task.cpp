#include "task.h"

QVariantList Task::asList() const
{
    QVariantList list;

    list.append(QVariant(this->getId()));
    list.append(QVariant(this->getCreatedDate()));
    list.append(QVariant(this->getModifiedDate()));
    list.append(QVariant(this->getDueDate()));
    list.append(QVariant(this->getStatus()));
    list.append(QVariant(this->getTitle()));
    list.append(QVariant(this->getDescription()));
    list.append(QVariant(this->getItemId()));

    return list;
}
