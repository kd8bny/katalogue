#include <QObject>
#include <QDebug>
#include <QVariantList>

#ifndef TASK_H
#define TASK_H


class Task
{

public:
    Task(int id);
    ~Task();

    int getId() const { return id; }

    QString getCreatedDate() const { return createdDate; }
    void setCreatedDate(const QString &createdDate_) { createdDate = createdDate_; }

    QString getModifiedDate() const { return modifiedDate; }
    void setModifiedDate(const QString &modifiedDate_) { modifiedDate = modifiedDate_; }

    QString getDueDate() const { return dueDate; }
    void setDueDate(const QString &dueDate_) { dueDate = dueDate_; }

    QString getTitle() const { return title; }
    void setTitle(const QString &title_) { title = title_; }

    QString getDescription() const { return description; }
    void setDescription(const QString &description_) { description = description_; }

    int getItemId() const { return itemId; }
    void setItemId(int itemId_) { itemId = itemId_; }

    QVariantList asList();

private:
    int id;
    QString createdDate;
    QString modifiedDate;
    QString dueDate;
    QString title;
    QString description;
    int itemId;
};

#endif
