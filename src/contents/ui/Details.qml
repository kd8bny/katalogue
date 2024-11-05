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
    property int menuItemHighlighted: 0

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
            highlighted: menuItemHighlighted == 0
            onClicked: {
                menuItemHighlighted = 0;
                pageStack.push("qrc:Attributes.qml", {
                    "entryItem": entryItem
                });
            }
        }

        Kirigami.SubtitleDelegate {
            Layout.fillWidth: true
            icon.name: "tag-events-symbolic"
            text: i18n("Events")
            highlighted: menuItemHighlighted == 1
            onClicked: {
                menuItemHighlighted = 1;
                pageStack.push("qrc:Events.qml", {
                    "entryItem": entryItem
                });
            }
        }

        Kirigami.SubtitleDelegate {
            Layout.fillWidth: true
            icon.name: "backgroundtool-symbolic"
            text: i18n("Notes")
            highlighted: menuItemHighlighted == 2
            onClicked: {
                menuItemHighlighted = 2;
                pageStack.push("qrc:Notes.qml", {
                    "entryItem": entryItem
                });
            }
        }

        Kirigami.SubtitleDelegate {
            Layout.fillWidth: true
            icon.name: "view-task"
            text: i18n("Tasks")
            highlighted: menuItemHighlighted == 3
            onClicked: {
                menuItemHighlighted = 3;
                pageStack.push("qrc:Tasks.qml", {
                    "entryItem": entryItem
                });
            }
        }

        Kirigami.SubtitleDelegate {
            Layout.fillWidth: true
            icon.name: "folder-documents-symbolic"
            text: i18n("Documents")
            highlighted: menuItemHighlighted == 4
            onClicked: {
                menuItemHighlighted = 4;
                pageStack.push("qrc:Documents.qml", {
                    "entryItem": entryItem
                });
            }
        }

        Kirigami.SubtitleDelegate {
            Layout.fillWidth: true
            icon.name: "extension-symbolic"
            text: i18n("Components")
            visible: !isComponentView
            highlighted: menuItemHighlighted == 5
            onClicked: {
                menuItemHighlighted = 5;
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
            highlighted: menuItemHighlighted == 6
            onClicked: {
                menuItemHighlighted = 6;
                pageStack.push('qrc:ItemInfoPage.qml', {
                    "entryItem": entryItem
                });
            }
        }

    }

}
