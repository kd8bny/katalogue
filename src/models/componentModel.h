#include <QObject>
#include <QSqlQueryModel>


class ComponentModel: public QSqlQueryModel
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

    explicit ComponentModel(QString itemId = "", QObject *parent = nullptr);
    ~ComponentModel();
    void refresh();

    // Override the method that will return the data
    QVariant data(const QModelIndex &index, int role) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;
    QString modelQuery;

//signals:

public slots:
    void updateModel(QString itemId);
    int getId(int row);
    QVariantList getRecord(int row);
};