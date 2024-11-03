#include <QObject>
#include <QSqlQueryModel>

#include "databaseSchema.h"
#include "itemDatabase.h"
#include "item.h"

#ifndef ITEM_MODEL_H
#define ITEM_MODEL_H

class ItemModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    enum Roles
    {
        rID = Qt::UserRole + 1,
        rNAME,
        rMAKE,
        rMODEL,
        rYEAR,
        rTYPE,
        rARCHIVED,
        rUSER_ORDER,
        rITEM_ID
    };

    explicit ItemModel(QObject *parent = nullptr);
    ~ItemModel() override = default;

    // Override the method that will return the data
    QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE int getId(int row) const;
    Q_INVOKABLE void setItemPosition(const int index, const int direction) const;

    QString modelQuery;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    const QString modelQueryBase = QStringLiteral(
                                       "SELECT id, %1, %2, %3, %4, %5, %6, %7 FROM %8 ")
                                       .arg(DatabaseSchema::NAME, DatabaseSchema::MAKE, DatabaseSchema::MODEL, DatabaseSchema::YEAR,
                                            DatabaseSchema::TYPE, DatabaseSchema::ARCHIVED, DatabaseSchema::USER_ORDER,
                                            DatabaseSchema::TABLE_ITEMS);
    const QString modelQueryParentItem = QStringLiteral("WHERE %1 IS NULL AND %2 IS 0 ")
                                             .arg(DatabaseSchema::KEY_ITEM_ID, DatabaseSchema::ARCHIVED);
    const QString modelQueryIncludeComponents = QStringLiteral("WHERE %2 IS 0 ").arg(DatabaseSchema::ARCHIVED);
    const QString modelQueryArchive = QStringLiteral("WHERE %1 IS 1 ").arg(DatabaseSchema::ARCHIVED);
    const QString modelQuerySortUser = QStringLiteral("ORDER BY %1 ASC NULLS LAST ").arg(DatabaseSchema::USER_ORDER);

Q_SIGNALS:
    void filterItem();
    void filterArchive();
    void filterComponent();
    void modelQueryChanged() const;

public Q_SLOTS:
    void setItemQuery();
    void setArchiveQuery();
    void setComponentQuery();
    void refreshModel();
};

#endif
