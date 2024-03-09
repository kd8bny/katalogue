#include <QObject>
#include <QDebug>

#ifndef EVENT_H
#define EVENT_H


class Event
{

public:
    explicit Event(int id);
    ~Event();

    int getId() const { return id; }

    QString getCreatedDate() const { return createdDate; }
    void setCreatedDate(const QString &createdDate_) { createdDate = createdDate_; }

    QString getModifiedDate() const { return modifiedDate; }
    void setModifiedDate(const QString &modifiedDate_) { modifiedDate = modifiedDate_; }

    QString getDate() const { return date; }
    void setDate(const QString &date_) { date = date_; }

    QString getEvent() const { return event; }
    void setEvent(const QString &event_) { event = event_; }

    float getCost() const { return cost; }
    void setCost(float cost_) { cost = cost_; }

    float getOdometer() const { return odometer; }
    void setOdometer(float odometer_) { odometer = odometer_; }

    QString getCategory() const { return category; }
    void setCategory(const QString &category_) { category = category_; }

    QString getComment() const { return comment; }
    void setComment(const QString &comment_) { comment = comment_; }

    int getItemId() const { return itemId; }
    void setItemId(int itemId_) { itemId = itemId_; }

    QVariantList asList();

private:
    int id;
    QString createdDate;
    QString modifiedDate;
    QString date;
    QString event;
    float cost;
    float odometer;
    QString category;
    QString comment;
    int itemId;
};

#endif
