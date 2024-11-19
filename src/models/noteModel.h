#include <QObject>
#include <QSqlQueryModel>

#include "databaseSchema.h"
#include "filterProxyModel.h"
#include "noteDatabase.h"
#include "entryFactory.h"
#include "note.h"

class NoteModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    enum Roles
    {
        rID = Qt::UserRole + 1,
        rTitle,
        rNote,
        rItemID,
    };

    enum class SortRole
    {
        DEFAULT, // Title
        NOTE_CONTENT,
    };
    Q_ENUM(SortRole);

    explicit NoteModel(QObject *parent = nullptr);
    ~NoteModel() override = default;

    // Override the method that will return the data
    QVariant data(const QModelIndex &index, int role) const override;
    Q_INVOKABLE int getId(int row) const;
    Q_INVOKABLE FilterProxyModel *getFilterProxyModel() { return filterProxyModel; }

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    const QString modelQueryBase = QStringLiteral("SELECT id, %1, %2, %3 FROM %4 ")
                                       .arg(DatabaseSchema::TITLE, DatabaseSchema::NOTE_CONTENT, DatabaseSchema::KEY_ITEM_ID,
                                            DatabaseSchema::TABLE_NOTES);

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
