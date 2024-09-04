#include <QObject>

#include "entry.h"

#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

class Attribute : public Entry
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString key READ getKey WRITE setKey)
    Q_PROPERTY(QString value READ getValue WRITE setValue)
    Q_PROPERTY(QString category READ getCategory WRITE setCategory)

public:
    Q_INVOKABLE using Entry::Entry;
    ~Attribute() override = default;

    QString getKey() const { return key; }
    void setKey(const QString &key_) { key = key_; }

    QString getValue() const { return value; }
    void setValue(const QString &value_) { value = value_; }

    QString getCategory() const { return category; }
    void setCategory(const QString &category_) { category = category_; }

private:
    QString key;
    QString value;
    QString category;
};

#endif
