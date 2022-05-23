#include <QObject>
#include <QSqlQueryModel>



class AttributeModel: public QSqlQueryModel
{
    Q_OBJECT

public:
    enum Roles {
        rID = Qt::UserRole + 1,
        rLabel,
        rKey,
        rValue,
        rItemID,
    };

    explicit AttributeModel(QObject *parent = nullptr);
    ~AttributeModel();
    void refresh();
    void setModelQuery();

    // Override the method that will return the data
    QVariant data(const QModelIndex &index, int role) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;
    QString modelQuery;

signals:

public slots:
    void setItemID(QString item_id);
    int getId(int row);
};