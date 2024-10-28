#include "documentIOHelper.h"
#include "documentDatabase.h"

#include <QStandardPaths>

DocumentIOHelper::DocumentIOHelper(QObject *parent)
    : QObject(parent)
{
}

bool DocumentIOHelper::loadFile(const QUrl filePath, Document *document) const
{
    DocumentDatabase documentDb;
    qDebug() << filePath << filePath.toLocalFile();

    if (QFile file(filePath.toLocalFile()); file.open(QIODevice::ReadOnly))
    {
        qDebug() << file.size();

        QByteArray fileBytes = file.readAll();
        document->setDataLazy(fileBytes);
        file.close();
    }
    else
    {
        qDebug() << "Failed to read file: " << file.error() << file.errorString();
    }

    return true;
}

QUrl DocumentIOHelper::writeFile(const QUrl filePath, const Document *document) const
{

    QUrl filePathFinal;
    if (filePath.toString() == QStringLiteral(""))
    {
        filePathFinal = QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QDir::separator() + document->getFileName());
    }
    else
    {
        filePathFinal = QUrl::fromLocalFile(filePath.toLocalFile() + QDir::separator() + document->getFileName());
    }
    qDebug() << filePathFinal;
    if (QFile file(filePathFinal.toLocalFile()); file.open(QIODevice::WriteOnly))
    {
        file.write(document->getDataLazy());
        file.close();
    }
    else
    {
        qDebug() << "Failed to write file: " << file.error() << file.errorString();
        return QUrl();
    }

    qDebug() << "File written to : " << filePathFinal;
    return filePathFinal;
}
