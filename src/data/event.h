#include <QObject>
#include <QDebug>

#ifndef EVENT_H
#define EVENT_H


class Event
{

public:
    Event(int id);
    ~Event();

    int getId() const { return id; }

    int getItemId() const { return itemId; }
    void setItemId(int itemId_) { itemId = itemId_; }

    QString getDate() const { return date; }
    void setDate(const QString &date_) { date = date_; }

    float getCost() const { return cost; }
    void setCost(float cost_) { cost = cost_; }

    float getOdometer() const { return odometer; }
    void setOdometer(float odometer_) { odometer = odometer_; }

    QString getCategory() const { return category; }
    void setCategory(const QString &category_) { category = category_; }

    QString getType() const { return type; }
    void setType(const QString &type_) { type = type_; }

    QString getComment() const { return comment; }
    void setComment(const QString &comment_) { comment = comment_; }

    QVariantList asList();

private:
    int id;
    int itemId;
    QString date;
    float cost;
    float odometer;
    QString category;
    QString type;
    QString comment;
};

#endif
