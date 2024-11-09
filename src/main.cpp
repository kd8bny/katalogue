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

#include <KAboutData>
#include <KLocalizedContext>
#include <KLocalizedString>

#include "about.h"
#include "katalogue.h"
#include "katalogueconfig.h"
#include "version-katalogue.h"
#include "constants/defaults.h"

#include "data/databaseInit.h"
#include "data/itemDatabase.h"
#include "data/attributeDatabase.h"
#include "data/eventDatabase.h"
#include "data/noteDatabase.h"
#include "data/taskDatabase.h"
#include "data/documentDatabase.h"
#include "utils/documentIOHelper.h"

#include "models/attributeModel.h"
#include "models/documentModel.h"
#include "models/eventModel.h"
#include "models/itemModel.h"
// #include "models/itemComponentModel.h"
#include "models/noteModel.h"
#include "models/taskModel.h"
#include "models/uniqueValueModel.h"
#include "models/uniqueValueModelFactory.h"
#include "models/itemModelFactory.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
    QCoreApplication::setApplicationName(QStringLiteral("katalogue"));
    KLocalizedString::setApplicationDomain("katalogue");

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
    QString qPath = QProcessEnvironment::systemEnvironment().value(QStringLiteral("KATALOGUE_DATA"), QLatin1String(""));
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

    // QML Data Types
    qmlRegisterType<Item>("com.kd8bny.katalogue.entries", 1, 0, "EntryItem");
    qmlRegisterType<Attribute>("com.kd8bny.katalogue.entries", 1, 0, "EntryAttribute");
    qmlRegisterType<Event>("com.kd8bny.katalogue.entries", 1, 0, "EntryEvent");
    qmlRegisterType<Note>("com.kd8bny.katalogue.entries", 1, 0, "EntryNote");
    qmlRegisterType<Task>("com.kd8bny.katalogue.entries", 1, 0, "EntryTask");
    qmlRegisterType<Document>("com.kd8bny.katalogue.entries", 1, 0, "EntryDocument");

    // Database and Data Interfaces
    if (DatabaseInit db; !db.connectKatalogueDb(qPath))
    {
        return -1;
    }

    DocumentIOHelper documentIOHelper;
    qmlRegisterSingletonInstance<DocumentIOHelper>("com.kd8bny.katalogue", 1, 0, "DocumentIOHelper", &documentIOHelper);

    ItemDatabase itemDatabase;
    qmlRegisterSingletonInstance<ItemDatabase>("com.kd8bny.katalogue", 1, 0, "ItemDatabase", &itemDatabase);
    AttributeDatabase attributeDatabase;
    qmlRegisterSingletonInstance<AttributeDatabase>(
        "com.kd8bny.katalogue", 1, 0, "AttributeDatabase", &attributeDatabase);
    EventDatabase eventDatabase;
    qmlRegisterSingletonInstance<EventDatabase>("com.kd8bny.katalogue", 1, 0, "EventDatabase", &eventDatabase);
    NoteDatabase noteDatabase;
    qmlRegisterSingletonInstance<NoteDatabase>("com.kd8bny.katalogue", 1, 0, "NoteDatabase", &noteDatabase);
    TaskDatabase taskDatabase;
    qmlRegisterSingletonInstance<TaskDatabase>("com.kd8bny.katalogue", 1, 0, "TaskDatabase", &taskDatabase);
    DocumentDatabase documentDatabase;
    qmlRegisterSingletonInstance<DocumentDatabase>("com.kd8bny.katalogue", 1, 0, "DocumentDatabase", &documentDatabase);

    // Models
    AttributeModel attributeModel;
    qmlRegisterSingletonInstance<AttributeModel>("com.kd8bny.katalogue", 1, 0, "AttributeModel", &attributeModel);
    DocumentModel documentModel;
    qmlRegisterSingletonInstance<DocumentModel>("com.kd8bny.katalogue", 1, 0, "DocumentModel", &documentModel);
    Defaults defaults; // TODO
    qmlRegisterSingletonInstance<Defaults>("com.kd8bny.katalogue", 1, 0, "Defaults", &defaults);
    EventModel eventModel;
    qmlRegisterSingletonInstance<EventModel>("com.kd8bny.katalogue", 1, 0, "EventModel", &eventModel);

    ItemModelFactory itemModelFactory;
    qmlRegisterSingletonInstance<ItemModelFactory>(
        "com.kd8bny.katalogue", 1, 0, "ItemModelFactory", &itemModelFactory);

    // qmlRegisterType<ItemModel>("com.kd8bny.katalogue", 1, 0, "ItemModel");
    // qmlRegisterUncreatableType<CustomQueryModel>("com.example", 1, 0, "CustomQueryModel", "Cannot create CustomQueryModel in QML");
    // ItemModel itemModel(ItemModel::ITEMS, QStringLiteral(""));
    // ItemModel *itemModel = ItemModelFactory().createItemModel();
    // qmlRegisterSingletonInstance<ItemModel>("com.kd8bny.katalogue", 1, 0, "ItemModel", itemModel);
    qmlRegisterUncreatableType<ItemModel>("com.kd8bny.katalogue", 1, 0, "ItemModel", QStringLiteral("reason"));
    NoteModel noteModel;
    qmlRegisterSingletonInstance<NoteModel>("com.kd8bny.katalogue", 1, 0, "NoteModel", &noteModel);
    TaskModel taskModel;
    qmlRegisterSingletonInstance<TaskModel>("com.kd8bny.katalogue", 1, 0, "TaskModel", &taskModel);

    // Unique Value Models
    UniqueValueModelFactory uniqueValueModelFactory;
    qmlRegisterSingletonInstance<UniqueValueModelFactory>(
        "com.kd8bny.katalogue", 1, 0, "UniqueValueModelFactory", &uniqueValueModelFactory);

    // ItemModelFactory itemModelFactory;
    // qmlRegisterSingletonInstance<ItemModelFactory>(
    //     "com.kd8bny.katalogue", 1, 0, "ItemModelFactory", &itemModelFactory);

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    return app.exec();
}
