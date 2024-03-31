#include <QObject>
#include <QDebug>
#include <QVariantList>

#ifndef NOTE_H
#define NOTE_H


class Note
{

public:
    explicit Note(int id);
    ~Note();

    int getId() const { return id; }

    QString getCreatedDate() const { return createdDate; }
    void setCreatedDate(const QString &createdDate_) { createdDate = createdDate_; }

    QString getModifiedDate() const { return modifiedDate; }
    void setModifiedDate(const QString &modifiedDate_) { modifiedDate = modifiedDate_; }

    QString getTitle() const { return title; }
    void setTitle(const QString &title_) { title = title_; }

    QString getNoteContent() const { return noteContent; }
    void setNoteContent(const QString &noteContent_) { noteContent = noteContent_; }

    int getItemId() const { return itemId; }
    void setItemId(int itemId_) { itemId = itemId_; }

    QVariantList asList();

private:
    int id;
    QString createdDate;
    QString modifiedDate;
    QString title;
    QString noteContent;
    int itemId;
};

#endif
