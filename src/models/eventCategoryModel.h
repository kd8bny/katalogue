#include <QObject>
#include <QSqlQueryModel>

#include "data/databaseSchema.h"

class EventCategoryModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit EventCategoryModel(QObject *parent = nullptr);
    ~EventCategoryModel();

private:
    const QString modelQuery = QStringLiteral("SELECT DISTINCT %1 FROM %2").arg(DatabaseSchema::CATEGORY, DatabaseSchema::TABLE_EVENTS);

Q_SIGNALS:
    void dataChanged();

public Q_SLOTS:
    void refresh();
};
