#include <QObject>
#include <QDebug>
#include <QVariantList>

#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H


class Attribute
{

public:
    explicit Attribute(int id);
    ~Attribute();

    int getId() const { return id; }

    QString getCreatedDate() const { return createdDate; }
    void setCreatedDate(const QString &createdDate_) { createdDate = createdDate_; }

    QString getModifiedDate() const { return modifiedDate; }
    void setModifiedDate(const QString &modifiedDate_) { modifiedDate = modifiedDate_; }

    QString getKey() const { return key; }
    void setKey(const QString &key_) { key = key_; }

    QString getValue() const { return value; }
    void setValue(const QString &value_) { value = value_; }

    QString getCategory() const { return category; }
    void setCategory(const QString &category_) { category = category_; }

    int getItemId() const { return itemId; }
    void setItemId(int itemId_) { itemId = itemId_; }

    QVariantList asList();

private:
    int id;
    QString createdDate;
    QString modifiedDate;
    QString key;
    QString value;
    QString category;
    int itemId;
};

#endif
