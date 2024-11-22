#include <QSortFilterProxyModel>

#ifndef FILTER_PROXY_H
#define FILTER_PROXY_H

class FilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit FilterProxyModel(QObject *parent = nullptr);
    ~FilterProxyModel() override = default;

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
