// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import com.kd8bny.katalogue


Kirigami.ScrollablePage {
    id: page

    required property int itemModelIndex  // Item Model index position
    required property int itemId          // Item Id from Database
    required property string itemName     // Item name field
    property bool isComponentView         // Component view or Item level view

    title: i18n(itemName)

    actions: [
        Kirigami.Action {
            text: i18n("Home")
            icon.name: "go-home-symbolic"
            tooltip: i18n("Go back to items page")
            onTriggered: {
                pageStack.clear()
                pageStack.push("qrc:Items.qml")
            }
        }
    ]

    Component.onCompleted: {
        // TODO enable and or fix?
        // pageStack.push("qrc:Attributes.qml", {"itemName": itemName, "itemId": itemId})
    }

    ColumnLayout {
        Kirigami.SubtitleDelegate {
            Layout.fillWidth: true

            icon.name: "view-list-details"
            text: i18n("Attributes")
            onClicked: pageStack.push("qrc:Attributes.qml", {"itemName": itemName, "itemId": itemId})
        }
        Kirigami.SubtitleDelegate {
            Layout.fillWidth: true

            icon.name: "view-calendar-agenda"
            text: i18n("Events")
            onClicked: pageStack.push("qrc:Events.qml", {"itemName": itemName, "itemId": itemId})
        }
        Kirigami.SubtitleDelegate {
            Layout.fillWidth: true

            icon.name: "view-list-details"
            text: i18n("Components")
            // TODO enabled: isComponentView
            onClicked: {
                pageStack.push("qrc:Items.qml", {"isComponentView": true})
                ItemComponentModel.filterComponent()
            }
        }
        Kirigami.SubtitleDelegate {
            Layout.fillWidth: true

            icon.name: "view-list-details"
            text: i18n("Notes")
            onClicked: pageStack.push("qrc:Notes.qml", {"itemName": itemName, "itemId": itemId})
        }
        Kirigami.SubtitleDelegate {
            Layout.fillWidth: true

            icon.name: "view-task"
            text: i18n("Tasks")
            // TODO onClicked: pageStack.push("qrc:Notes.qml", {"itemName": itemName, "itemId": itemId})
        }
        Kirigami.SubtitleDelegate {
            Layout.fillWidth: true

            icon.name: "view-list-details"
            text: i18n("Details")
            onClicked: pageStack.push('qrc:ItemInfoPage.qml', {"itemModelIndex": itemModelIndex})
        }
    }
}
