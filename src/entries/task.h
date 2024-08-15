#include <QObject>
#include <QtQml>

#include "entry.h"

#ifndef TASK_H
#define TASK_H

class Task : public Entry
{
    Q_OBJECT
    Q_PROPERTY(QString dueDate READ getDueDate WRITE setDueDate)
    Q_PROPERTY(QString status READ getStatus WRITE setStatus)
    Q_PROPERTY(QString title READ getTitle WRITE setTitle)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription)

public:
    Q_INVOKABLE using Entry::Entry;
    ~Task() override = default;

    QString getDueDate() const { return dueDate; }
    void setDueDate(const QString &dueDate_) { dueDate = dueDate_; }

    QString getStatus() const { return status; }
    void setStatus(const QString &status_) { status = status_; }

    QString getTitle() const { return title; }
    void setTitle(const QString &title_) { title = title_; }

    QString getDescription() const { return description; }
    void setDescription(const QString &description_) { description = description_; }

private:
    QString dueDate;
    QString status;
    QString title;
    QString description;
};

#endif
