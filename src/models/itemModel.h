#include <QObject>
#include <QSqlQueryModel>

class ItemModel: public QSqlQueryModel
{
    Q_OBJECT

public:
    enum Roles {
        rID = Qt::UserRole + 1,
        rUID,
        rNAME,
        rMAKE,
        rMODEL,
        rYEAR,
        rGroup,
        rCategory,
        rArchived
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
    int getId(int row);
};