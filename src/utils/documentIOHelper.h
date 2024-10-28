#include <QObject>

#include "document.h"

class DocumentIOHelper : public QObject
{
    Q_OBJECT

public:
    explicit DocumentIOHelper(QObject *parent = nullptr);
    ~DocumentIOHelper() override = default;

    Q_INVOKABLE bool loadFile(const QUrl filePath, Document *document) const;
    Q_INVOKABLE QUrl writeFile(const QUrl filePath, const Document *document) const;
};
