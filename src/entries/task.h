#include <QObject>
#include <QDebug>
#include <QVariantList>

#include "entry.h"

#ifndef TASK_H
#define TASK_H

class Task : public Entry
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit Task(QObject *parent = nullptr)
        : Entry(parent){};
    ~Task();

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
