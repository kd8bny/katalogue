#include <QObject>
#include <QSqlQueryModel>

class AttributeModel: public QSqlQueryModel
{
    Q_OBJECT

public:
    enum Roles {
        rID = Qt::UserRole + 1,
        rUuid,
        rLabel,
        rKey,
        rValue,
    };

    explicit AttributeModel(QObject *parent = nullptr);
    ~AttributeModel();
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