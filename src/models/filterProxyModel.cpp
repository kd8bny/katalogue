#include <QSqlRecord>

#include "filterProxyModel.h"

FilterProxyModel::FilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    QObject::connect(this, &FilterProxyModel::setFilterString, this,
                     &FilterProxyModel::onSetFilterString);
}

bool FilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
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

void FilterProxyModel::onSetFilterString(const QString &filter)
{
    filterString = filter;
    invalidateRowsFilter();
}
