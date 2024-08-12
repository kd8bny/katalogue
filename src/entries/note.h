#include <QObject>
#include <QDebug>
#include <QVariantList>

#include "entry.h"

#ifndef NOTE_H
#define NOTE_H

class Note : public Entry
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit Note(QObject *parent = nullptr)
        : Entry(parent){};
    ~Note();

    QString getTitle() const { return title; }
    void setTitle(const QString &title_) { title = title_; }

    QString getNoteContent() const { return noteContent; }
    void setNoteContent(const QString &noteContent_) { noteContent = noteContent_; }

private:
    QString title;
    QString noteContent;
};

#endif
