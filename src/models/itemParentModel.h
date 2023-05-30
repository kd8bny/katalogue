#include <QObject>
#include <QSqlQueryModel>

#include "../database.h"


class ItemParentModel: public QSqlQueryModel
{
    Q_OBJECT

public:

    explicit ItemParentModel(QObject *parent = nullptr);
    ~ItemParentModel();

signals:
    void dataChanged();

public slots:
    void refresh();

private:
    const QString modelQuery = QString(
        "SELECT DISTINCT %1 FROM %2 WHERE %3 IS NULL").arg(
            NAME, TABLE_ITEMS, KEY_ITEM_ID);

};
