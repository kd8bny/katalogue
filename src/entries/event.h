#include <QObject>
#include <QDebug>
#include <QVariantList>

#include "entry.h"

#ifndef EVENT_H
#define EVENT_H

class Event : public Entry
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString date READ getDate WRITE setDate)
    Q_PROPERTY(QString event READ getEvent WRITE setEvent)
    Q_PROPERTY(QString servicer READ getServicer WRITE setServicer)
    Q_PROPERTY(double cost READ getCost WRITE setCost)
    Q_PROPERTY(double increment READ getIncrement WRITE setIncrement)
    Q_PROPERTY(QString category READ getCategory WRITE setCategory)
    Q_PROPERTY(QString comment READ getComment WRITE setComment)

public:
    using Entry::Entry;
    ~Event() override = default;

    QString getDate() const { return date; }
    void setDate(const QString &date_) { date = date_; }

    QString getEvent() const { return event; }
    void setEvent(const QString &event_) { event = event_; }

    QString getServicer() const { return servicer; }
    void setServicer(QString servicer_) { servicer = servicer_; }

    double getCost() const { return cost; }
    void setCost(double cost_) { cost = cost_; }

    double getIncrement() const { return increment; }
    void setIncrement(double increment_) { increment = increment_; }

    QString getCategory() const { return category; }
    void setCategory(const QString &category_) { category = category_; }

    QString getComment() const { return comment; }
    void setComment(const QString &comment_) { comment = comment_; }

private:
    QString date;
    QString event;
    QString servicer;
    double cost;
    double increment;
    QString category;
    QString comment;
};

#endif
