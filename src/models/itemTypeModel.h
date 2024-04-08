#include <QObject>
#include <QSqlQueryModel>

#include "data/database.h"


class ItemTypeModel: public QSqlQueryModel
{
    Q_OBJECT

public:

    explicit ItemTypeModel(QObject *parent = nullptr);
    ~ItemTypeModel();

private:
    const QString modelQuery = QStringLiteral("SELECT DISTINCT %1 FROM %2").arg(
        Database::TYPE, Database::TABLE_ITEMS);

Q_SIGNALS:
    void dataChanged();

public Q_SLOTS:
    void refresh();

};
