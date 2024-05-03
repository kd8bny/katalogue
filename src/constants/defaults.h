#include <QObject>
#include <QList>

#ifndef DEFAULT_H
#define DEFAULT_H

class Defaults: public QObject
{
    Q_OBJECT

public:
    Defaults();
    ~Defaults();

    Q_INVOKABLE QList<QString> getValidTaskStatus() const { return validTaskStatus; }

private:
    const QList<QString> validTaskStatus = QList({
        QStringLiteral("Todo"),
        QStringLiteral("In Progress"),
        QStringLiteral("Done"),
        QStringLiteral("Blocked"),
        QStringLiteral("Overdue"),
    });
};

#endif
