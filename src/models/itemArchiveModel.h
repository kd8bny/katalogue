#include <QObject>
#include <QSqlQueryModel>


class ItemArchiveModel: public QSqlQueryModel
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

    explicit ItemArchiveModel(QObject *parent = nullptr);
    ~ItemArchiveModel();
    void refresh();

    // Override the method that will return the data
    QVariant data(const QModelIndex &index, int role) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

//signals:

public slots:
    void updateModel();
    int getId(int row);
    QVariantList getRecord(int row);
};