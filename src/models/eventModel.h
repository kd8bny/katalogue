#include <QObject>
#include <QSqlQueryModel>

#include "databaseSchema.h"
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

    explicit EventModel(QObject *parent = nullptr);
    ~EventModel() = default;

    // Override the method that will return the data
    QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE int getId(int row) const;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    const QString modelQueryBase = QStringLiteral("SELECT id, %1, %2, %3, %4, %5, %6 FROM %7")
                                       .arg(DatabaseSchema::DATE, DatabaseSchema::EVENT, DatabaseSchema::COST,
                                            DatabaseSchema::INCREMENT, DatabaseSchema::CATEGORY, DatabaseSchema::COMMENT,
                                            DatabaseSchema::TABLE_EVENTS);

    const QString modelQuerySetId = QStringLiteral(" WHERE %1=%2");

    QString modelQuery;

Q_SIGNALS:
    void filterItemId(QString itemId);
    void dataChanged();

public Q_SLOTS:
    void setItemIdQuery(QString itemId);
    void refresh();
};
