#include <QObject>
#include <QSqlQueryModel>

class EventModel: public QSqlQueryModel
{
    Q_OBJECT

public:
    enum Roles {
        rID = Qt::UserRole + 1,
        rUid,
        rDate,
        rTask,
        rCost,
        rType,
        rCategory,
        rComment,
    };

    explicit EventModel(QObject *parent = nullptr);
    ~EventModel();
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