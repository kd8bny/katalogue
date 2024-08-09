#include <QObject>
#include <QDebug>
#include <QVariantList>

#include "entry.h"

#ifndef NOTE_H
#define NOTE_H

class Note : public Entry
{

public:
    using Entry::Entry;
    ~Note() override = default;

    QString getTitle() const { return title; }
    void setTitle(const QString &title_) { title = title_; }

    QString getNoteContent() const { return noteContent; }
    void setNoteContent(const QString &noteContent_) { noteContent = noteContent_; }

    QVariantList asList() const override;

private:
    QString title;
    QString noteContent;
};

#endif
