#include <QObject>
#include <QSqlQueryModel>

#include "data/database.h"


class ItemParentModel: public QSqlQueryModel
{
    Q_OBJECT

public:

    explicit ItemParentModel(QObject *parent = nullptr);
    ~ItemParentModel();

    Q_INVOKABLE int getId(int row);

private:
    // First select is the combox values, second is the Id of the record
    // Ignore components and archived Items
    const QString modelQuery = QStringLiteral(
        "SELECT DISTINCT %1, id FROM %2 WHERE %3 IS NULL AND %4 IS 0").arg(
            Database::NAME, Database::TABLE_ITEMS, Database::KEY_ITEM_ID, Database::ARCHIVED);

Q_SIGNALS:
    void dataChanged();

public Q_SLOTS:
    void refresh();
};
