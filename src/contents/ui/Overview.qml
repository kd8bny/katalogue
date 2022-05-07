// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami


Kirigami.ScrollablePage {
    id: overviewPage

    Layout.fillWidth: true

    title: i18n("Overview")

    //Close the drawer with the back button
    onBackRequested: {
        if (sheet.sheetOpen) {
            event.accepted = true;
            sheet.close();
        }
    }

    actions {
        main: Kirigami.Action {
            iconName: "go-home"
            tooltip: i18n("Overview")
            enabled: root.pageStack.lastVisibleItem != overviewPage
            onTriggered: root.pageStack.pop(-1)
        }
        contextualActions: [
            Kirigami.Action {
                text: i18n("Add Event")
                icon.name: "list-add"
                tooltip: i18n("Add new event")
                onTriggered: {
                    addEventSheet.open()
                }
            },
            Kirigami.Action {
                text: i18n("Add Item")
                icon.name: "list-add"
                tooltip: i18n("Add new item")
                onTriggered: {
                    addItemSheet.open()
                }
            }
        ]
    }

    ColumnLayout {
        Kirigami.Heading {
            //Layout.alignment: Qt.AlignCenter
            //text: counter == 0 ? i18n("Hello, World!") : counter
            text: i18n("Welcome to katalogue\nA tool to track all you maintenance needs")
        }
        Kirigami.CardsLayout {
            id: layout
            width: overviewPage.width
            Kirigami.Card {
                headerOrientation: Qt.Horizontal
                actions: [
                    Kirigami.Action {
                        text: qsTr("Action1")
                        icon.name: "add-placemark"
                    },
                    Kirigami.Action {
                        text: qsTr("Action2")
                        icon.name: "address-book-new-symbolic"
                    }
                ]
                // banner {
                //     source: Qt.resolvedUrl("../banner.jpg")
                //     title: "Title"
                // }
                contentItem: Controls.Label {
                    wrapMode: Text.WordWrap
                    text: qsTr("A card can optionally have horizontal orientation.\n In this case will be wider than tall, so is fit to be used also in a ColumnLayout.\nIf you need to put it in a CardsLayout, it will have by default a columnSpan of 2 (which can be overridden).")
                }
            }
        }

    }

    AddEventSheet {
        id: addEventSheet
    }

    AddItemSheet {
        id: addItemSheet
    }
}