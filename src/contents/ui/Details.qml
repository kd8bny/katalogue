// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import com.kd8bny.katalogue
import com.kd8bny.katalogue.entries
import org.kde.kirigami as Kirigami

Kirigami.ScrollablePage {
    id: itemsDetailPage

    required property EntryItem entryItem
    property bool isComponentView: false

    Component.onCompleted: {
        if (entryItem.itemId != 0)
            isComponentView = true;

    }
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
        // Default Page Push
        Timer {
            interval: 50
            running: true
            repeat: false
            onTriggered: {
                pageStack.push("qrc:Attributes.qml", {
                    "entryItem": entryItem
                });
            }
        }

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
            visible: !isComponentView
            onClicked: {
                pageStack.push("qrc:Items.qml", {
                    "entryComponent": entryItem,
                    "isComponentView": true
                });
                ItemComponentModel.filterParentItemId(entryItem.id);
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
