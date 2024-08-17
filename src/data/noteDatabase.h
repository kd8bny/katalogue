#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>

#include "databaseSchema.h"
#include "databaseUtils.h"
#include "entryFactory.h"
#include "note.h"

#ifndef NOTE_DATABASE_H
#define NOTE_DATABASE_H

class NoteDatabase : public QObject
{
    Q_OBJECT

public:
    explicit NoteDatabase(QObject *parent = nullptr);
    ~NoteDatabase() override = default;

    Q_INVOKABLE bool insertEntry(const Note *note) const;
    Q_INVOKABLE bool updateEntry(const Note *note) const;
    Q_INVOKABLE bool deleteEntryById(const int id) const;
    Q_INVOKABLE Note *getEntryById(const int id = 0) const;
};

#endif
