#include <QObject>
#include <QSqlQueryModel>

#include "data/databaseSchema.h"
#include "data/taskDatabase.h"
#include "data/entries/task.h"

class TaskModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    enum Roles
    {
        rID = Qt::UserRole + 1,
        rDueDate,
        rStatus,
        rTitle,
        rDescription,
        rItemID,
    };

    explicit TaskModel(QObject *parent = nullptr);
    ~TaskModel();

    // Override the method that will return the data
    QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE void resetItemId();
    Q_INVOKABLE void setItemId(QString itemId);
    Q_INVOKABLE int getId(int row);
    Task getRecord(int row);
    Q_INVOKABLE QVariantList getRecordAsList(int row);
    Q_INVOKABLE bool setRecord(int taskIndex, QString dueDate, QString status, QString title, QString description, int itemId);
    Q_INVOKABLE bool deleteRecord(int eventId);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    const QString modelQueryBase = QStringLiteral("SELECT id, %1, %2, %3, %4, %5 FROM %6 ").arg(DatabaseSchema::DUE_DATE, DatabaseSchema::STATUS, DatabaseSchema::TITLE, DatabaseSchema::DESCRIPTION, DatabaseSchema::KEY_ITEM_ID, DatabaseSchema::TABLE_TASKS);

    const QString modelQuerySetId = QStringLiteral(" WHERE %1=%2");

    QString modelQuery;

Q_SIGNALS:
    void dataChanged();

public Q_SLOTS:
    void refresh();
};
