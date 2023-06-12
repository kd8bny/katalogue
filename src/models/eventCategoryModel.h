#include <QObject>
#include <QSqlQueryModel>

#include "data/database.h"


class EventCategoryModel: public QSqlQueryModel
{
    Q_OBJECT

public:

    explicit EventCategoryModel(QObject *parent = nullptr);
    ~EventCategoryModel();

signals:
    void dataChanged();

public slots:
    void refresh();

private:
    const QString modelQuery = QString(
        "SELECT DISTINCT %1 FROM %2").arg(CATEGORY, TABLE_EVENTS);

};
