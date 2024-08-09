#include <QObject>
#include <QSqlQueryModel>
#include <QSqlQuery>

#include "data/databaseSchema.h"
#include "data/attributeDatabase.h"
#include "data/entries/attribute.h"

class AttributeModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    enum Roles
    {
        rID = Qt::UserRole + 1,
        rKey,
        rValue,
        rCategory,
        rItemID,
    };

    explicit AttributeModel(QObject *parent = nullptr);
    ~AttributeModel();

    // Override the method that will return the data
    QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE void setItemId(QString itemId);
    Q_INVOKABLE int getId(int row);
    Attribute getRecord(int row);
    Q_INVOKABLE QVariantList getRecordAsList(int row);
    Q_INVOKABLE bool setRecord(int attributeIndex, QString key, QString value,
                               QString category, int itemId);
    Q_INVOKABLE bool deleteRecord(int eventId);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    const QString modelQueryBase = QStringLiteral("SELECT id, %1, %2, %3, %4 FROM %5 ").arg(DatabaseSchema::KEY, DatabaseSchema::VALUE, DatabaseSchema::CATEGORY, DatabaseSchema::KEY_ITEM_ID, DatabaseSchema::TABLE_ATTRIBUTES);

    const QString modelQuerySetId = QStringLiteral(" WHERE %1=%2");

    QString modelQuery;

Q_SIGNALS:
    void dataChanged();

public Q_SLOTS:
    void refresh();
};
