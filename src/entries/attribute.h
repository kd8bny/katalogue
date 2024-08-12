#include <QObject>
#include <QDebug>
#include <QVariantList>

#include "entry.h"

#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

class Attribute : public Entry
{
    Q_OBJECT

public:
    Q_INVOKABLE using Entry::Entry;
    //     Q_INVOKABLE explicit Attribute(QObject *parent = nullptr)
    //     : Entry(parent){};
    ~Attribute();

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
