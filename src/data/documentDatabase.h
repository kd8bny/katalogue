#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QString>
#include <QStringBuilder>

#include "databaseSchema.h"
#include "databaseUtils.h"
#include "entryFactory.h"
#include "document.h"

#ifndef DOCUMENT_DATABASE_H
#define DOCUMENT_DATABASE_H

class DocumentDatabase : public QObject
{
    Q_OBJECT

public:
    explicit DocumentDatabase(QObject *parent = nullptr);
    ~DocumentDatabase() override = default;

    Q_INVOKABLE int insertEntry(const Document *document) const;
    Q_INVOKABLE bool updateEntry(const Document *document) const;
    Q_INVOKABLE bool deleteEntryById(const int id) const;
    Q_INVOKABLE Document *getEntryById(const int id = 0) const;

    bool insertDocumentBLOB(const int id, const QByteArray fileBytes) const;
};

#endif
