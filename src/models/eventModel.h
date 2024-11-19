#include <QObject>
#include <QSqlQueryModel>

#include "databaseSchema.h"
#include "filterProxyModel.h"
#include "eventDatabase.h"
#include "event.h"

class EventModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    enum Roles
    {
        rID = Qt::UserRole + 1,
        rDate,
        rEvent,
        rCost,
        rIncrement,
        rCategory,
        rComment,
        rItemID,
    };

    enum class SortRole
    {
        DEFAULT, // Date
        EVENT,
        CATEGORY,
    };
    Q_ENUM(SortRole);

    explicit EventModel(QObject *parent = nullptr);
    ~EventModel() override = default;

    // Override the method that will return the data
    QVariant data(const QModelIndex &index, int role) const override;
    Q_INVOKABLE int getId(int row) const;
    Q_INVOKABLE FilterProxyModel *getFilterProxyModel();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    const QString modelQueryBase = QStringLiteral("SELECT id, %1, %2, %3, %4, %5, %6 FROM %7 ")
                                       .arg(DatabaseSchema::DATE, DatabaseSchema::EVENT, DatabaseSchema::COST,
                                            DatabaseSchema::INCREMENT, DatabaseSchema::CATEGORY, DatabaseSchema::COMMENT,
                                            DatabaseSchema::TABLE_EVENTS);

    QString modelQuery;

    QString itemId;
    SortRole sortRole = SortRole::DEFAULT;
    QString sortOrder = DatabaseSchema::ORDER_ASC;

Q_SIGNALS:
    void setItemId(QString id);
    void setSortRole(SortRole sortBy);
    void setSortOrder(Qt::SortOrder sortOrder);
    void modelQueryChanged();

public Q_SLOTS:
    void onSetItemId(QString id);
    void onSetSortRole(SortRole sortBy);
    void onSetSortOrder(Qt::SortOrder sortOrder);
    void onModelQueryChanged();
};
