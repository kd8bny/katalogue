#include <QObject>
#include <QSqlQueryModel>

#include "data/databaseSchema.h"

class AttributeCategoryModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit AttributeCategoryModel(QObject *parent = nullptr);
    ~AttributeCategoryModel();

private:
    const QString modelQuery = QStringLiteral(
                                   "SELECT DISTINCT %1 FROM %2")
                                   .arg(DatabaseSchema::CATEGORY, DatabaseSchema::TABLE_ATTRIBUTES);

Q_SIGNALS:
    void dataChanged();

public Q_SLOTS:
    void refresh();
};
