#include <QObject>
#include <QSqlQueryModel>

#include "databaseSchema.h"
#include "taskDatabase.h"
#include "entryFactory.h"
#include "task.h"

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
    ~TaskModel() override = default;

    // Override the method that will return the data
    QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE int getId(int row) const;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    const QString modelQueryBase = QStringLiteral("SELECT id, %1, %2, %3, %4, %5 FROM %6 ")
                                       .arg(DatabaseSchema::DUE_DATE, DatabaseSchema::STATUS, DatabaseSchema::TITLE,
                                            DatabaseSchema::DESCRIPTION, DatabaseSchema::KEY_ITEM_ID,
                                            DatabaseSchema::TABLE_TASKS);

    const QString modelQuerySetId = QStringLiteral(" WHERE %1=%2");

    QString modelQuery;

Q_SIGNALS:
    void filterItemId(QString itemId);
    void resetFilterItemId();
    void modelQueryChanged();

public Q_SLOTS:
    void setItemIdQuery(QString itemId);
    void resetItemIdQuery();
    void refreshModel();
};
