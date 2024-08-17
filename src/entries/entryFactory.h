#include <QObject>

#include "entry.h"
#include "item.h"
#include "attribute.h"
#include "event.h"
#include "task.h"
#include "note.h"

#ifndef ENTRY_FACTORY_H
#define ENTRY_FACTORY_H

class EntryFactory
{
public:
    EntryFactory() = default;
    ~EntryFactory() = default;

    Item *createItem() const
    {
        return new Item();
    };

    Attribute *createAttribute() const
    {
        return new Attribute();
    };

    Event *createEvent() const
    {
        return new Event();
    };

    Note *createNote() const
    {
        return new Note();
    };

    Task *createTask() const
    {
        return new Task();
    };
};

#endif
