#include <QObject>
#include <QtQml>

#include "entry.h"

#ifndef DOCUMENT_H
#define DOCUMENT_H

class Document : public Entry
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(QString fileName READ getFileName WRITE setFileName)

    Q_PROPERTY(int eventId READ getEventId WRITE setEventId)
    Q_PROPERTY(int noteId READ getNoteId WRITE setNoteId)
    Q_PROPERTY(int taskId READ getTaskId WRITE setTaskId)

public:
    Q_INVOKABLE using Entry::Entry;
    ~Document() override = default;

    QString getName() const { return name; }
    void setName(const QString &name_) { name = name_; }

    QString getFileName() const { return fileName; }
    void setFileName(const QString &fileName_) { fileName = fileName_; }

    QString getData() const { return data; }
    void setData(const QString &data_) { data = data_; }

    int getEventId() const { return eventId; }
    void setEventId(int eventId_) { eventId = eventId_; }

    int getNoteId() const { return NoteId; }
    void setNoteId(int eventId_) { NoteId = NoteId_; }

    int getTaskId() const { return taskId; }
    void setTaskId(int taskId_) { taskId = taskId_; }

private:
    QString name;
    QString fileName;
    QString data;
    int eventId;
    int noteId;
    int taskId;
};

#endif
