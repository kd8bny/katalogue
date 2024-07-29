#include <QObject>
#include <QSqlQueryModel>

#include "data/database.h"
#include "data/event.h"

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
    ~EventModel();

    // Override the method that will return the data
    QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE void setItemId(QString itemId);
    Q_INVOKABLE int getId(int row);
    Event getRecord(int row);
    Q_INVOKABLE QVariantList getRecordAsList(int row);
    Q_INVOKABLE bool setRecord(int eventIndex, QString date, QString eventName,
                               float cost, float increment, QString category, QString comment, int itemId);
    Q_INVOKABLE bool deleteRecord(int eventId);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    const QString modelQueryBase = QStringLiteral("SELECT id, %1, %2, %3, %4, %5, %6 FROM %7")
                                       .arg(Database::DATE, Database::EVENT, Database::COST, Database::INCREMENT,
                                            Database::CATEGORY, Database::COMMENT, Database::TABLE_EVENTS);

    const QString modelQuerySetId = QStringLiteral(" WHERE %1=%2");

    QString modelQuery;

Q_SIGNALS:
    void dataChanged();

public Q_SLOTS:
    void refresh();
};
