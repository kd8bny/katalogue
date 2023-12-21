#include <QObject>
#include <QDebug>

#ifndef TASK_H
#define TASK_H


class Task
{

public:
    Task(int id);
    ~Task();

    int getId() const { return id; }

    QString getTitle() const { return title; }
    void setTitle(const QString &title_) { title = title_; }

    QString getDescription() const { return description; }
    void setDescription(const QString &description_) { description = description_; }

    QString getDueDate() const { return dueDate; }
    void setDueDate(const QString &dueDate_) { dueDate = dueDate_; }

    int getItemId() const { return itemId; }
    void setItemId(int itemId_) { itemId = itemId_; }

    QVariantList asList();

private:
    int id;
    int itemId;
    QString title;
    QString description;
    QString dueDate;
};

#endif
