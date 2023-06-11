#include <QObject>
#include <QSqlQueryModel>

#include "../database.h"
#include "data/item.h"


class ItemModel: public QSqlQueryModel
{
    Q_OBJECT

public:
    enum Roles {
        rID = Qt::UserRole + 1,
        rNAME,
        rMAKE,
        rMODEL,
        rYEAR,
        rTYPE,
        rARCHIVED,
        rPARENT
    };

    explicit ItemModel(QObject *parent = nullptr);
    ~ItemModel();

    // Override the method that will return the data
    QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE int getId(int row);
    Item getRecord(int row);
    Q_INVOKABLE QVariantList getRecordAsList(int row);
    Q_INVOKABLE bool setRecord(int itemIndex, QString name, QString make,
        QString model, int year, QString type, int archived, int parent);
    Q_INVOKABLE bool deleteRecord(int itemId);

protected:
    QHash<int, QByteArray> roleNames() const override;

signals:
    void filterItem();
    void filterArchive();
    void filterComponent();
    void dataChanged();

public slots:
    void setItemQuery();
    void setArchiveQuery();
    void setComponentQuery();
    void refresh();

private:
    const QString modelQueryBase = QString(
        "SELECT id, %1, %2, %3, %4, %5, %6 FROM %7").arg(
            NAME, MAKE, MODEL, YEAR, TYPE, ARCHIVED, TABLE_ITEMS);
    const QString modelQueryItem = QString(
        " WHERE %1 IS NULL AND %2 IS 0").arg(KEY_ITEM_ID, ARCHIVED);
    const QString modelQueryArchive = QString(
        " WHERE %1 IS 1").arg(ARCHIVED);
    const QString modelQueryComponent = QString(
        " WHERE %1 IS NOT NULL AND %2 IS 0").arg(KEY_ITEM_ID, ARCHIVED);

    QString modelQuery;

};
