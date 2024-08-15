#include <QObject>
#include <QSqlQueryModel>

#include "databaseSchema.h"
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

    explicit NoteModel(QObject *parent = nullptr);
    ~NoteModel() override = default;

    // Override the method that will return the data
    QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE int getId(int row) const;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    const QString modelQueryBase = QStringLiteral("SELECT id, %1, %2, %3 FROM %4")
                                       .arg(DatabaseSchema::TITLE, DatabaseSchema::NOTE_CONTENT, DatabaseSchema::KEY_ITEM_ID,
                                            DatabaseSchema::TABLE_NOTES);

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
