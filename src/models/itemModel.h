#include <QObject>
#include <QSqlQueryModel>

#include "databaseSchema.h"
#include "itemDatabase.h"
#include "item.h"

#include "filterProxyModel.h"

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

    enum class ItemModelType
    {
        ITEMS,
        COMPONENTS,
        ARCHIVE
    };
    Q_ENUM(ItemModelType);

    explicit ItemModel(ItemModelType itemModelType, QString modelQueryBase, QObject *parent = nullptr);
    ~ItemModel() override = default;

    // Override the method that will return the data
    QVariant data(const QModelIndex &index, int role) const override;
    Q_INVOKABLE int getId(int row) const;

    ItemModelType getItemModelType() const { return itemModelType; }
    Q_INVOKABLE FilterProxyModel *getFilterProxyModel() { return filterProxyModel; }

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QString modelQueryBase;
    ItemModelType itemModelType;
    FilterProxyModel *filterProxyModel;

    bool includeComponent = false;
    Roles sortField = ItemModel::rUSER_ORDER;
    QString sortOrder = DatabaseSchema::ORDER_ASC;

Q_SIGNALS:
    void setItemPosition(const int index, const int direction);
    void setSortRole(Roles sortBy);
    void setSortOrder(Qt::SortOrder sortOrder);
    void toggleComponents();
    void modelQueryChanged();

public Q_SLOTS:
    void onSetItemPosition(const int index, const int direction);
    void onSetSortRole(Roles sortBy);
    void onSetSortOrder(Qt::SortOrder sortOrder);
    void onToggleComponents();
    void onModelQueryChanged();
};

#endif
