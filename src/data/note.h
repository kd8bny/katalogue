#include <QObject>
#include <QDebug>

#ifndef NOTE_H
#define NOTE_H


class Note
{

public:
    Note(int id);
    ~Note();

    int getId() const { return id; }

    QString getTitle() const { return title; }
    void setTitle(const QString &title_) { title = title_; }

    QString getNoteContent() const { return noteContent; }
    void setNoteContent(const QString &noteContent_) { noteContent = noteContent_; }

    int getItemId() const { return itemId; }
    void setItemId(int itemId_) { itemId = itemId_; }

    QVariantList asList();

private:
    int id;
    int itemId;
    QString title;
    QString noteContent;
};

#endif
