// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import com.kd8bny.katalogue
import com.kd8bny.katalogue.entries
import org.kde.kirigami as Kirigami

Kirigami.ScrollablePage {
    // Component.onCompleted: {
    //     // TODO enable and or fix?
    //     // pageStack.push("qrc:Attributes.qml", {"itemName": itemName, "itemId": itemId})
    // }

    id: itemsDetailPage

    // property bool isComponentView         // Component view or Item level view
    required property EntryItem entryItem

    title: i18n(entryItem.name)
    actions: [
        Kirigami.Action {
            text: i18n("Home")
            icon.name: "go-home-symbolic"
            tooltip: i18n("Go back to items page")
            onTriggered: {
                pageStack.clear();
                pageStack.push("qrc:Items.qml");
            }
        }
    ]

    ColumnLayout {
        Kirigami.SubtitleDelegate {
            Layout.fillWidth: true
            icon.name: "view-list-details"
            text: i18n("Attributes")
            onClicked: pageStack.push("qrc:Attributes.qml", {
                "entryItem": entryItem
            })
        }

        Kirigami.SubtitleDelegate {
            Layout.fillWidth: true
            icon.name: "tag-events-symbolic"
            text: i18n("Events")
            onClicked: pageStack.push("qrc:Events.qml", {
                "entryItem": entryItem
            })
        }

        Kirigami.SubtitleDelegate {
            Layout.fillWidth: true
            icon.name: "backgroundtool-symbolic"
            text: i18n("Notes")
            onClicked: pageStack.push("qrc:Notes.qml", {
                "entryItem": entryItem
            })
        }

        Kirigami.SubtitleDelegate {
            Layout.fillWidth: true
            icon.name: "view-task"
            text: i18n("Tasks")
            onClicked: pageStack.push("qrc:Tasks.qml", {
                "entryItem": entryItem
            })
        }

        Kirigami.SubtitleDelegate {
            Layout.fillWidth: true
            icon.name: "extension-symbolic"
            text: i18n("Components")
            // visible: !isComponentView
            onClicked: {
                pageStack.push("qrc:Items.qml", {
                    "isComponentView": true
                });
                ItemComponentModel.filterParentItemId(itemId);
            }
        }

        Kirigami.SubtitleDelegate {
            Layout.fillWidth: true
            icon.name: "file-catalog-symbolic"
            text: i18n("Details")
            onClicked: pageStack.push('qrc:ItemInfoPage.qml', {
                "entryItem": entryItem
            })
        }

    }

}
