#include <QObject>
#include <QSqlQueryModel>

#include "itemModel.h"


class ItemComponentModel: public ItemModel
{
    Q_OBJECT

public:
    explicit ItemComponentModel();
    ~ItemComponentModel();

};
