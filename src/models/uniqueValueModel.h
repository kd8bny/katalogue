#include <QObject>
#include <QSqlQueryModel>
#include <QSqlRecord>

#include "data/databaseSchema.h"

#ifndef UNIQUE_VALUE_MODEL_H
#define UNIQUE_VALUE_MODEL_H

class UniqueValueModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit UniqueValueModel(QObject *parent = nullptr);
    ~UniqueValueModel() override = default;
    void setModelQuery(QString modelQuery);
    Q_INVOKABLE int getId(int row);

private:
    QString modelQuery;

Q_SIGNALS:
    void modelQueryChanged();

public Q_SLOTS:
    void onModelQueryChanged();
};

#endif
