// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2024 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

import com.kd8bny.katalogue

Kirigami.ApplicationWindow {
    id: root

    title: i18n("Katalogue")

    minimumWidth: Kirigami.Units.gridUnit * 20
    minimumHeight: Kirigami.Units.gridUnit * 20

    // onClosing: Controller.saveWindowGeometry(root)

    onWidthChanged: saveWindowGeometryTimer.restart()
    onHeightChanged: saveWindowGeometryTimer.restart()
    onXChanged: saveWindowGeometryTimer.restart()
    onYChanged: saveWindowGeometryTimer.restart()

    // This timer allows to batch update the window size change to reduce
    // the io load and also work around the fact that x/y/width/height are
    // changed when loading the page and overwrite the saved geometry from
    // the previous session.
    Timer {
        id: saveWindowGeometryTimer
        interval: 1000
        // onTriggered: Controller.saveWindowGeometry(root)
    }

    globalDrawer: Kirigami.GlobalDrawer {
        id: globalDrawer

        title: i18n("katalogue")
        titleIcon: "applications-graphics"
        //isMenu: !root.isMobile
        modal: false
        collapsible: true
        showHeaderWhenCollapsed: true
        collapsed: true

        header: RowLayout {
            Layout.fillWidth: true
            Controls.ToolButton {
                icon.name: "application-menu"
                visible: globalDrawer.collapsible
                checked: !globalDrawer.collapsed
                onClicked: globalDrawer.collapsed = !globalDrawer.collapsed
            }
        }

        actions: [
            Kirigami.Action {
                text: i18n("Items")
                tooltip: i18n("Katalouged Items")
                icon.name: "file-catalog-symbolic"
                onTriggered: {
                    pageStack.clear()
                    pageStack.push('qrc:Items.qml')
                    ItemModel.filterItem()
                }
            },
            Kirigami.Action {
                text: i18n("Tasks")
                tooltip: i18n("View Tasks")
                icon.name: "view-task"
                onTriggered: {
                    TaskModel.resetItemId()
                    pageStack.clear()
                    pageStack.push('qrc:Tasks.qml')
                }
            },
            Kirigami.Action {
                text: i18n("Notes")
                tooltip: i18n("View Notes")
                icon.name: "backgroundtool-symbolic"
                onTriggered: {
                    NoteModel.resetItemId()
                    pageStack.clear()
                    pageStack.push('qrc:Notes.qml')
                }
            },
            Kirigami.Action {
                text: i18n("Archive")
                tooltip: i18n("View Archived Items")
                icon.name: "folder-tar-symbolic"
                onTriggered: {
                    pageStack.clear()
                    pageStack.push('qrc:Items.qml')
                    ItemModel.filterArchive()
                }
            },
            Kirigami.Action {
                text: i18n("About")
                tooltip: i18n("About Katalogue")
                icon.name: "help-about"
                onTriggered: pageStack.layers.push('qrc:About.qml')
            }
        ]
    }

    contextDrawer: Kirigami.ContextDrawer {
        id: contextDrawer
    }

    Items {
        id: itemsPage
    }

    AttributeInfoSheet {
        id: attributeInfoSheet
    }

    EventInfoSheet {
        id: eventInfoSheet
    }

    pageStack.globalToolBar.style: Kirigami.ApplicationHeaderStyle.ToolBar
    pageStack.initialPage: itemsPage

}
