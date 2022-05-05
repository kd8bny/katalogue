/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>
*/

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QUrl>
#include <QtQml>

#include "about.h"
#include "app.h"
#include "database.h"
#include "version-katalogue.h"
#include <KAboutData>
#include <KLocalizedContext>
#include <KLocalizedString>

#include "katalogueconfig.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
    QCoreApplication::setApplicationName(QStringLiteral("katalogue"));

    KAboutData aboutData(
                         // The program name used internally.
                         QStringLiteral("katalogue"),
                         // A displayable program name string.
                         i18nc("@title", "katalogue"),
                         // The program version string.
                         QStringLiteral(KATALOGUE_VERSION_STRING),
                         // Short description of what the app does.
                         i18n("Maintenance Logging System"),
                         // The license this code is released under.
                         KAboutLicense::GPL,
                         // Copyright Statement.
                         i18n("(c) 2022"));
    aboutData.addAuthor(i18nc("@info:credit", "AUTHOR"), i18nc("@info:credit", "Author Role"), QStringLiteral("kd8bny@gmail.com"), QStringLiteral("https://yourwebsite.com"));
    KAboutData::setApplicationData(aboutData);

    QQmlApplicationEngine engine;

    auto config = katalogueConfig::self();

    qmlRegisterSingletonInstance("org.kde.katalogue", 1, 0, "Config", config);

    AboutType about;
    qmlRegisterSingletonInstance("org.kde.katalogue", 1, 0, "AboutType", &about);

    App application;
    qmlRegisterSingletonInstance("org.kde.katalogue", 1, 0, "App", &application);

    Database database;
    database.connect();
    qmlRegisterSingletonInstance<Database>("com.kd8bny.katalogue", 1, 0, "Database", &database);

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
