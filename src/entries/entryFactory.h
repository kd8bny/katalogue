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
    std::unique_ptr<Item> createItem() const
    {
        return std::make_unique<Item>();
    };

    std::unique_ptr<Attribute> createAttribute() const
    {
        return std::make_unique<Attribute>();
    };

    std::unique_ptr<Event> createEvent() const
    {
        return std::make_unique<Event>();
    };

    std::unique_ptr<Note> createNote() const
    {
        return std::make_unique<Note>();
    };

    std::unique_ptr<Task> createTask() const
    {
        return std::make_unique<Task>();
    };
};

#endif
