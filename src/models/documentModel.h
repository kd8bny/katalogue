#include <QObject>
#include <QSqlQueryModel>

#include "databaseSchema.h"
#include "filterProxyModel.h"
#include "documentDatabase.h"
#include "entryFactory.h"
#include "document.h"

class DocumentModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    enum Roles
    {
        rID = Qt::UserRole + 1,
        rName,
        rFileName,
    };

    enum class SortRole
    {
        DEFAULT, // Name
        FILENAME,
    };
    Q_ENUM(SortRole);

    explicit DocumentModel(QObject *parent = nullptr);
    ~DocumentModel() override = default;

    // Override the method that will return the data
    QVariant data(const QModelIndex &index, int role) const override;
    Q_INVOKABLE int getId(int row) const;
    Q_INVOKABLE FilterProxyModel *getFilterProxyModel() { return filterProxyModel; }

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    const QString modelQueryBase = QStringLiteral("SELECT id, %1, %2 FROM %4 ")
                                       .arg(DatabaseSchema::NAME, DatabaseSchema::FILE_NAME,
                                            DatabaseSchema::TABLE_DOCUMENTS);

    QString modelQuery;

    QString itemId;
    FilterProxyModel *filterProxyModel;
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
