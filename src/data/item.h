#include <QObject>
#include <QDebug>
#include <QVariantList>

#ifndef ITEM_H
#define ITEM_H


class Item
{

public:
    explicit Item(int id);
    ~Item();

    int getId() const { return id; }

    QString getCreatedDate() const { return createdDate; }
    void setCreatedDate(const QString &createdDate_) { createdDate = createdDate_; }

    QString getModifiedDate() const { return modifiedDate; }
    void setModifiedDate(const QString &modifiedDate_) { modifiedDate = modifiedDate_; }

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

    int getParent() const { return parent; }
    void setParent(const int &parent_) { parent = parent_; }

    QVariantList asList();

private:
    int id;
    QString createdDate;
    QString modifiedDate;
    QString name;
    QString make;
    QString model;
    int year;
    QString type;
    bool archived;
    int userOrder;
    int parent;
};

#endif
