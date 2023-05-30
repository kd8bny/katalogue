#include <QObject>
#include <QSqlQueryModel>

#include "../database.h"


class AttributeCategoryModel: public QSqlQueryModel
{
    Q_OBJECT

public:

    explicit AttributeCategoryModel(QObject *parent = nullptr);
    ~AttributeCategoryModel();

signals:
    void dataChanged();

public slots:
    void refresh();

private:
    const QString modelQuery = QString(
        "SELECT DISTINCT %1 FROM %2").arg(CATEGORY, TABLE_ATTRIBUTES);

};
