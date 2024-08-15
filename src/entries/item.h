#include <QObject>
#include <QtQml>

#include "entry.h"

#ifndef ITEM_H
#define ITEM_H

class Item : public Entry
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(QString make READ getMake WRITE setMake)
    Q_PROPERTY(QString model READ getModel WRITE setModel)
    Q_PROPERTY(int year READ getYear WRITE setYear)
    Q_PROPERTY(QString type READ getType WRITE setType)
    Q_PROPERTY(bool archived READ getArchived WRITE setArchived)
    Q_PROPERTY(int userOrder READ getUserOrder WRITE setUserOrder)

public:
    Q_INVOKABLE using Entry::Entry;
    ~Item() override = default;

    QString getName() const { return name; }
    void setName(const QString &name_) { name = name_; }

    QString getMake() const { return make; }
    void setMake(const QString &make_) { make = make_; }

    QString getModel() const { return model; }
    void setModel(const QString &model_) { model = model_; }

    int getYear() const { return year; }
    void setYear(int year_) { year = year_; }

    QString getType() const { return type; }
    void setType(const QString &type_) { type = type_; }

    bool getArchived() const { return archived; }
    void setArchived(const bool &archived_) { archived = archived_; }

    int getUserOrder() const { return userOrder; }
    void setUserOrder(const int &userOrder_) { userOrder = userOrder_; }

private:
    QString name;
    QString make;
    QString model;
    int year;
    QString type;
    bool archived = false;
    int userOrder = 0;
};

#endif
