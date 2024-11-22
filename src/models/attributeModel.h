#include <QObject>
#include <QSqlQueryModel>

#include "databaseSchema.h"
#include "attributeDatabase.h"
#include "attribute.h"

#ifndef ATTRIBUTE_MODEL_H
#define ATTRIBUTE_MODEL_H

class AttributeModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    enum Roles
    {
        rID = Qt::UserRole + 1,
        rKey,
        rValue,
        rCategory,
        rItemID,
    };

    explicit AttributeModel(QObject *parent = nullptr);
    ~AttributeModel() override = default;

    // Override the method that will return the data
    QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE int getId(int row) const;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    const QString modelQueryBase = QStringLiteral("SELECT id, %1, %2, %3, %4 FROM %5 ")
                                       .arg(DatabaseSchema::KEY, DatabaseSchema::VALUE, DatabaseSchema::CATEGORY,
                                            DatabaseSchema::KEY_ITEM_ID, DatabaseSchema::TABLE_ATTRIBUTES);

    const QString modelQuerySetId = QStringLiteral(" WHERE %1=%2");

    QString modelQuery;

Q_SIGNALS:
    void filterItemId(QString itemId);
    void modelQueryChanged();

public Q_SLOTS:
    void setItemIdQuery(QString itemId);
    void onModelQueryChanged();
};

#endif
