#include <QObject>

#ifndef ENTRY_H
#define ENTRY_H

class Entry
{
public:
    explicit Entry(int id = 0) : id(id) {}
    virtual ~Entry() = default;

    int getId() const { return id; }

    QString getCreatedDate() const { return createdDate; }
    void setCreatedDate(const QString &createdDate_) { createdDate = createdDate_; }

    QString getModifiedDate() const { return modifiedDate; }
    void setModifiedDate(const QString &modifiedDate_) { modifiedDate = modifiedDate_; }

    int getItemId() const { return itemId; }
    void setItemId(int itemId_) { itemId = itemId_; }

    virtual QVariantList asList() const = 0;

private:
    int id;
    QString createdDate;
    QString modifiedDate;
    int itemId;
};

#endif
