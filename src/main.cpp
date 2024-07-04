/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2024 Daryl Bennett <kd8bny@gmail.com>
*/
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QUrl>
#include <QtQml>
#include <QStandardPaths>
#include <QDir>
#include <QProcessEnvironment>

#include "about.h"
#include "katalogue.h"
#include "version-katalogue.h"
#include "constants/defaults.h"
#include "data/database.h"
#include "models/attributeModel.h"
#include "models/attributeCategoryModel.h"
#include "models/eventModel.h"
#include "models/eventCategoryModel.h"
#include "models/itemModel.h"
#include "models/itemComponentModel.h"
#include "models/itemTypeModel.h"
#include "models/itemParentModel.h"
#include "models/noteModel.h"
#include "models/taskModel.h"

#include <KAboutData>
#include <KLocalizedContext>
#include <KLocalizedString>

#include "katalogueconfig.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
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
        i18n("(c) 2023"));

    aboutData.addAuthor(i18nc("@info:credit", "Daryl Bennett"), i18nc("@info:credit", "Developer"), QStringLiteral("kd8bny@gmail.com"));
    KAboutData::setApplicationData(aboutData);

    // Set Application Directories
    QString qPath = QProcessEnvironment::systemEnvironment().value(QStringLiteral("KATALOGUE_DATA"), QStringLiteral(""));
    if (qPath.length() == 0)
    {
        qPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    }
    qDebug() << qPath;

    if (!QDir(qPath).mkpath(qPath))
    {
        qDebug() << "Path not writeable " << qPath;

        return -1;
    }

    QQmlApplicationEngine engine;

    auto config = katalogueConfig::self();
    qmlRegisterSingletonInstance("org.kde.katalogue", 1, 0, "Config", config);

    AboutType about;
    qmlRegisterSingletonInstance("org.kde.katalogue", 1, 0, "AboutType", &about);

    App application;
    qmlRegisterSingletonInstance("org.kde.katalogue", 1, 0, "App", &application);

    Database database;
    if (!database.connectKatalogueDb(qPath))
    {
        return -1;
    }

    qmlRegisterSingletonInstance<Database>("com.kd8bny.katalogue", 1, 0, "Database", &database);

    AttributeModel attributeModel;
    qmlRegisterSingletonInstance<AttributeModel>("com.kd8bny.katalogue", 1, 0, "AttributeModel", &attributeModel);

    AttributeCategoryModel attributeCategoryModel;
    qmlRegisterSingletonInstance<AttributeCategoryModel>("com.kd8bny.katalogue", 1, 0, "AttributeCategoryModel", &attributeCategoryModel);

    Defaults defaults;
    qmlRegisterSingletonInstance<Defaults>("com.kd8bny.katalogue", 1, 0, "Defaults", &defaults);

    EventModel eventModel;
    qmlRegisterSingletonInstance<EventModel>("com.kd8bny.katalogue", 1, 0, "EventModel", &eventModel);

    EventCategoryModel eventCategoryModel;
    qmlRegisterSingletonInstance<EventCategoryModel>("com.kd8bny.katalogue", 1, 0, "EventCategoryModel", &eventCategoryModel);

    ItemModel itemModel;
    qmlRegisterSingletonInstance<ItemModel>("com.kd8bny.katalogue", 1, 0, "ItemModel", &itemModel);

    ItemComponentModel itemComponentModel;
    qmlRegisterSingletonInstance<ItemComponentModel>("com.kd8bny.katalogue", 1, 0, "ItemComponentModel", &itemComponentModel);

    ItemTypeModel itemTypeModel;
    qmlRegisterSingletonInstance<ItemTypeModel>("com.kd8bny.katalogue", 1, 0, "ItemTypeModel", &itemTypeModel);

    ItemParentModel itemParentModel;
    qmlRegisterSingletonInstance<ItemParentModel>("com.kd8bny.katalogue", 1, 0, "ItemParentModel", &itemParentModel);

    NoteModel noteModel;
    qmlRegisterSingletonInstance<NoteModel>("com.kd8bny.katalogue", 1, 0, "NoteModel", &noteModel);

    TaskModel taskModel;
    qmlRegisterSingletonInstance<TaskModel>("com.kd8bny.katalogue", 1, 0, "TaskModel", &taskModel);

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    return app.exec();
}
