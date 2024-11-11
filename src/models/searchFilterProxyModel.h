#include <QSortFilterProxyModel>

#ifndef SEARCH_FILTER_PROXY_H
#define SEARCH_FILTER_PROXY_H

class SearchFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit SearchFilterProxyModel(QObject *parent = nullptr);
    Q_INVOKABLE int getId(int row) const;

protected:
    // Override filterAcceptsRow to implement custom filtering
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString filterString;

Q_SIGNALS:
    void setFilterString(const QString &filter);
public Q_SLOTS:
    void onSetFilterString(const QString &filter);
};

#endif
