#include <QObject>
#include <QtQml>

#ifndef ENTRY_H
#define ENTRY_H

class Entry : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int id READ getId)
    Q_PROPERTY(QString createdDate READ getCreatedDate)
    Q_PROPERTY(QString modifiedDate READ getModifiedDate)
    Q_PROPERTY(int itemId READ getItemId WRITE setItemId)

public:
    using QObject::QObject;
    ;
    ~Entry() override = default;

    int getId() const { return id; }
    void setId(const int &id_) { id = id_; }

    QString getCreatedDate() const { return createdDate; }
    void setCreatedDate(const QString &createdDate_) { createdDate = createdDate_; }

    QString getModifiedDate() const { return modifiedDate; }
    void setModifiedDate(const QString &modifiedDate_) { modifiedDate = modifiedDate_; }

    int getItemId() const { return itemId; }
    void setItemId(int itemId_) { itemId = itemId_; }

private:
    int id = 0;
    QString createdDate;
    QString modifiedDate;
    int itemId;
};

#endif
