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

    QString getNote() const { return note; }
    void setNote(const QString &note_) { note = note_; }

    QVariantList asList();

private:
    int id;
    QString title;
    QString note;
};

#endif
