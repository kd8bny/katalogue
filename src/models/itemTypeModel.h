#include <QObject>
#include <QSqlQueryModel>

#include "data/databaseSchema.h"

class ItemTypeModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit ItemTypeModel(QObject *parent = nullptr);
    ~ItemTypeModel();

private:
    const QString modelQuery = QStringLiteral("SELECT DISTINCT %1 FROM %2").arg(DatabaseSchema::TYPE, DatabaseSchema::TABLE_ITEMS);

Q_SIGNALS:
    void dataChanged();

public Q_SLOTS:
    void refresh();
};
