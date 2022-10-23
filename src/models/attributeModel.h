#include <QObject>
#include <QSqlQueryModel>

class AttributeModel: public QSqlQueryModel
{
    Q_OBJECT

public:
    enum Roles {
        rID = Qt::UserRole + 1,
        rKey,
        rValue,
        rCategory,
        rItemID,
    };

    explicit AttributeModel(QObject *parent = nullptr);
    ~AttributeModel();
    void refresh();

    // Override the method that will return the data
    QVariant data(const QModelIndex &index, int role) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;
    QString modelQuery;

signals:

public slots:
    void updateModel();
    void setItemId(QString item_id);
    int getId(int row);
    QVariantList getRecord(int row);
    QVariantList getCategories();
};
