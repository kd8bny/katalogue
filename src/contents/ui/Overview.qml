// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami


Kirigami.Page {
    id: overviewPage

    Layout.fillWidth: true

    title: i18n("Overview")


    actions {
        main: Kirigami.Action {
            iconName: "go-home"
            enabled: root.pageStack.lastVisibleItem != pageRoot
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
                    addEventSheet.open()
                }
            }
        ]
    }

    ColumnLayout {
        width: page.width

        anchors.centerIn: parent

        Kirigami.Heading {
            Layout.alignment: Qt.AlignCenter
            //text: counter == 0 ? i18n("Hello, World!") : counter
            text: i18n("Welcome to katalogue\nA tool to track all you maintenance needs")
        }

    }

    AddEventSheet {
        id: addEventSheet
    }
}