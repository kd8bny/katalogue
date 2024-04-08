#include <QObject>
#include <QSqlQueryModel>

#include "data/database.h"


class EventCategoryModel: public QSqlQueryModel
{
    Q_OBJECT

public:

    explicit EventCategoryModel(QObject *parent = nullptr);
    ~EventCategoryModel();

private:
    const QString modelQuery = QStringLiteral("SELECT DISTINCT %1 FROM %2").arg(
        Database::CATEGORY, Database::TABLE_EVENTS);

Q_SIGNALS:
    void dataChanged();

public Q_SLOTS:
    void refresh();

};
