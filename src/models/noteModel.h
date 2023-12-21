#include <QObject>
#include <QSqlQueryModel>

#include "data/database.h"
#include "data/note.h"

class NoteModel: public QSqlQueryModel
{
    Q_OBJECT

public:
    enum Roles {
        rID = Qt::UserRole + 1,
        rTitle,
        rNote,
        rItemID,
    };

    explicit NoteModel(QObject *parent = nullptr);
    ~NoteModel();

    // Override the method that will return the data
    QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE void setItemId(QString itemId);
    Q_INVOKABLE int getId(int row);
    Note getRecord(int row);
    Q_INVOKABLE QVariantList getRecordAsList(int row);
    Q_INVOKABLE bool setRecord(int noteIndex, QString title, QString note, int itemId);
    Q_INVOKABLE bool deleteRecord(int eventId);

protected:
    QHash<int, QByteArray> roleNames() const override;


signals:
    void dataChanged();

public slots:
    void refresh();

private:
    const QString modelQueryBase = QString("SELECT id, %1, %2, %3 FROM %5 ").arg(
        TITLE, NOTE, KEY_ITEM_ID, TABLE_NOTES);

    const QString modelQuerySetId = QString(" WHERE %1=%2");

    QString modelQuery;
};
