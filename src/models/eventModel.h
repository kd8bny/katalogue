#include <QObject>
#include <QSqlQueryModel>

class EventModel: public QSqlQueryModel
{
    Q_OBJECT

public:
    enum Roles {
        rID = Qt::UserRole + 1,
        rDate,
        rEvent,
        rCost,
        rType,
        rCategory,
        rComment,
        rItemID,
    };

    explicit EventModel(QObject *parent = nullptr);
    ~EventModel();
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
