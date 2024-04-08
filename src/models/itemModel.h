#include <QObject>
#include <QSqlQueryModel>

#include "data/database.h"
#include "data/item.h"

#ifndef ITEM_MODEL_H
#define ITEM_MODEL_H


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

private:
    const QString modelQueryBase = QStringLiteral(
        "SELECT id, %1, %2, %3, %4, %5, %6 FROM %7").arg(
            Database::NAME, Database::MAKE, Database::MODEL, Database::YEAR, Database::TYPE, Database::ARCHIVED, Database::TABLE_ITEMS);
    const QString modelQueryItem = QStringLiteral(
        " WHERE %1 IS NULL AND %2 IS 0").arg(Database::KEY_ITEM_ID, Database::ARCHIVED);
    const QString modelQueryArchive = QStringLiteral(
        " WHERE %1 IS 1").arg(Database::ARCHIVED);
    const QString modelQueryComponent = QStringLiteral(
        " WHERE %1 IS NOT NULL AND %2 IS 0").arg(Database::KEY_ITEM_ID, Database::ARCHIVED);

    QString modelQuery;

Q_SIGNALS:
    void filterItem();
    void filterArchive();
    void filterComponent();
    void dataChanged();

public Q_SLOTS:
    void setItemQuery();
    void setArchiveQuery();
    void setComponentQuery();
    void refresh();
};

#endif
