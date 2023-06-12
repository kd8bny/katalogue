#include <QObject>
#include <QSqlQueryModel>

#include "data/database.h"


class ItemTypeModel: public QSqlQueryModel
{
    Q_OBJECT

public:

    explicit ItemTypeModel(QObject *parent = nullptr);
    ~ItemTypeModel();

signals:
    void dataChanged();

public slots:
    void refresh();

private:
    const QString modelQuery = QString(
        "SELECT DISTINCT %1 FROM %2").arg(TYPE, TABLE_ITEMS);

};
