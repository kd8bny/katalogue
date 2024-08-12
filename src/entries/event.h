#include <QObject>
#include <QDebug>
#include <QVariantList>

#include "entry.h"

#ifndef EVENT_H
#define EVENT_H

class Event : public Entry
{
    Q_OBJECT

public:
    using Entry::Entry;
    // explicit Event();
    // ~Event() override = default;
    Q_INVOKABLE explicit Event(QObject *parent = nullptr)
        : Entry(parent){};
    ~Event();

    QString getDate() const { return date; }
    void setDate(const QString &date_) { date = date_; }

    QString getEvent() const { return event; }
    void setEvent(const QString &event_) { event = event_; }

    float getCost() const { return cost; }
    void setCost(float cost_) { cost = cost_; }

    float getIncrement() const { return increment; }
    void setIncrement(float increment_) { increment = increment_; }

    QString getCategory() const { return category; }
    void setCategory(const QString &category_) { category = category_; }

    QString getComment() const { return comment; }
    void setComment(const QString &comment_) { comment = comment_; }

private:
    QString date;
    QString event;
    float cost;
    float increment;
    QString category;
    QString comment;
};

#endif
