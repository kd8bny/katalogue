#include <QObject>
#include <QSqlQueryModel>

#include "databaseSchema.h"
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

    explicit DocumentModel(QObject *parent = nullptr);
    ~DocumentModel() override = default;

    // Override the method that will return the data
    QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE int getId(int row) const;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    const QString modelQueryBase = QStringLiteral("SELECT id, %1, %2 FROM %4")
                                       .arg(DatabaseSchema::NAME, DatabaseSchema::FILE_NAME,
                                            DatabaseSchema::TABLE_DOCUMENTS);

    const QString modelQuerySetId = QStringLiteral(" WHERE %1=%2");

    QString modelQuery;

Q_SIGNALS:
    void filterItemId(QString itemId);
    void resetFilterItemId();
    void dataChanged();

public Q_SLOTS:
    void setItemIdQuery(QString itemId);
    void resetItemIdQuery();
    void refresh();
};
