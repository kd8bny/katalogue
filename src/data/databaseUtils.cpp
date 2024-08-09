#include "databaseUtils.h"

namespace DatabaseUtils
{
    QString getCurrentDateTime()
    {
        /*
         * Utility for created and modified timestamps
         */
        QDateTime dt = QDateTime::currentDateTime();
        dt.setTimeSpec(Qt::UTC);

        QString date = QDateTime::currentDateTime().toString(Qt::ISODate);

        return date;
    }
}
