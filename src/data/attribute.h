#include <QObject>
#include <QDebug>

#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H


class Attribute
{

public:
    Attribute(int id);
    ~Attribute();

    int getId() const { return id; }

    QVariantList asList();

    QString getKey() const { return key; }
    void setKey(const QString &key_) { key = key_; }

    QString getValue() const { return value; }
    void setValue(const QString &value_) { value = value_; }

    QString getCategory() const { return category; }
    void setCategory(const QString &category_) { category = category_; }

    int getItemId() const { return itemId; }
    void setItemId(int itemId_) { itemId = itemId_; }

private:
    int id;
    int itemId;
    QString key;
    QString value;
    QString category;

};

#endif
