#include <QObject>
#include <QDebug>

#ifndef TODO_H
#define TODO_H


class Todo
{

public:
    Todo(int id);
    ~Todo();

    int getId() const { return id; }

    QString getTitle() const { return title; }
    void setTitle(const QString &title_) { title = title_; }

    QString getDescription() const { return description; }
    void setDescription(const QString &description_) { description = description_; }

    QString getDueDate() const { return dueDate; }
    void setDueDate(const QString &dueDate_) { dueDate = dueDate_; }

    QVariantList asList();

private:
    int id;
    QString title;
    QString description;
    QString dueDate;
};

#endif
