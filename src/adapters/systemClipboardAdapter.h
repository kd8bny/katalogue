#include <QObject>
#include <QtQml>
#include <QGuiApplication>
#include <QClipboard>

#ifndef SYSTEM_CLIPBOARD_ADAPTER_H
#define SYSTEM_CLIPBOARD_ADAPTER_H

class SystemClipboardAdapter : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE using QObject::QObject;
    ~SystemClipboardAdapter() override = default;

    Q_INVOKABLE void sendStringToSystemClipboard(const QString string) { this->clipboard->setText(string); }

private:
    QClipboard *clipboard = QGuiApplication::clipboard();
};

#endif
