#include <QObject>
#include <QtQml>

#include "entry.h"

#ifndef NOTE_H
#define NOTE_H

class Note : public Entry
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString title READ getTitle WRITE setTitle)
    Q_PROPERTY(QString noteContent READ getNoteContent WRITE setNoteContent)

public:
    Q_INVOKABLE using Entry::Entry;
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
