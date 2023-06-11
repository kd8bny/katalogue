#include <QObject>
#include <QSqlQueryModel>

#include "../database.h"
#include "data/event.h"

class EventModel: public QSqlQueryModel
{
    Q_OBJECT

public:
    enum Roles {
        rID = Qt::UserRole + 1,
        rDate,
        rEvent,
        rCost,
        rOdometer,
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
        float cost, float odometer, QString category, QString comment, int itemId);
    Q_INVOKABLE bool deleteRecord(int eventId);

protected:
    QHash<int, QByteArray> roleNames() const override;

signals:
    void dataChanged();

public slots:
    void refresh();

private:
    const QString modelQueryBase = QString(
        "SELECT id, %1, %2, %3, %4, %5, %6 FROM %7").arg(
            DATE, EVENT, COST, ODOMETER, CATEGORY, COMMENT, TABLE_EVENTS);

    const QString modelQuerySetId = QString(" WHERE %1=%2");

    QString modelQuery;
};
