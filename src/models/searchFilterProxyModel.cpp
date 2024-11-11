#include <QSqlRecord>

#include "searchFilterProxyModel.h"
#include "itemModel.h"

SearchFilterProxyModel::SearchFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    QObject::connect(this, &SearchFilterProxyModel::setFilterString, this,
                     &SearchFilterProxyModel::onSetFilterString);
}

bool SearchFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (filterString.isEmpty())
    {
        return true;
    }

    QHash modelRoleNames = sourceModel()->roleNames();
    for (auto mrn = modelRoleNames.begin(); mrn != modelRoleNames.end(); ++mrn)
    {
        QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
        QString data = sourceModel()->data(index, mrn.key()).toString();

        if (data.contains(filterString, Qt::CaseInsensitive))
        {
            return true;
        }
    }

    return false;
}

int SearchFilterProxyModel::getId(int row) const
{
    QModelIndex index = mapToSource(this->index(row, 0));
    int data = sourceModel()->data(index, ItemModel::rID).toInt();

    return data;
}

void SearchFilterProxyModel::onSetFilterString(const QString &filter)
{
    filterString = filter;
    invalidateRowsFilter();
}
