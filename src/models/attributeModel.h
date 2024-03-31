#include <QObject>
#include <QSqlQueryModel>
#include <QSqlQuery>

#include "data/database.h"
#include "data/attribute.h"

// #include "AttributeModel.moc"

class AttributeModel: public QSqlQueryModel
{
    Q_OBJECT

public:
    enum Roles {
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

public slots:
    void refresh();

// signals:
//     void dataChanged();

// private:
//     const QString modelQueryBase = QStringLiteral("SELECT id, %1, %2, %3, %4 FROM %5 ").arg(
//         Database->KEY, VALUE, CATEGORY, KEY_ITEM_ID, TABLE_ATTRIBUTES);

//     const QString modelQuerySetId = QStringLiteral(" WHERE %1=%2");

    // QString modelQuery;
};


