#include <QObject>
#include <QSqlQueryModel>
#include <QSqlRecord>

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
    void refresh();

    // Override the method that will return the data
    QVariant data(const QModelIndex &index, int role) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

//signals:

public slots:
    void updateModel();
    void setModelComponents();
    int getId(int row);
    QVariantList getItemData(QString item_id);
    QVariantList getItemTypes();
    QVariantList getItemParents();
};