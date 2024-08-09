#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>

#include "databaseSchema.h"
#include "databaseUtils.h"
#include "entries/note.h"

#ifndef NOTE_DATABASE_H
#define NOTE_DATABASE_H

class NoteDatabase : public QObject
{
    Q_OBJECT

public:
    explicit NoteDatabase(QObject *parent = nullptr);
    ~NoteDatabase() override;

    bool insertEntry(const Note &note) const;
    bool updateEntry(const Note &note) const;
    bool deleteEntryById(const int id) const;
};

#endif
