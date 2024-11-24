// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2024 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import com.kd8bny.katalogue
import org.kde.kirigami as Kirigami

Kirigami.ApplicationWindow {
    // onClosing: Controller.saveWindowGeometry(root)

    id: root

    title: i18n("Katalogue")
    minimumWidth: Kirigami.Units.gridUnit * 20
    minimumHeight: Kirigami.Units.gridUnit * 20
    onWidthChanged: saveWindowGeometryTimer.restart()
    onHeightChanged: saveWindowGeometryTimer.restart()
    onXChanged: saveWindowGeometryTimer.restart()
    onYChanged: saveWindowGeometryTimer.restart()
    pageStack.initialPage: itemsPage

    // This timer allows to batch update the window size change to reduce
    // the io load and also work around the fact that x/y/width/height are
    // changed when loading the page and overwrite the saved geometry from
    // the previous session.
    Timer {
        // onTriggered: Controller.saveWindowGeometry(root)

        id: saveWindowGeometryTimer

        interval: 1000
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

    globalDrawer: Kirigami.GlobalDrawer {
        id: globalDrawer

        title: i18n("katalogue")
        titleIcon: "applications-graphics"
        modal: false
        collapsible: true
        showHeaderWhenCollapsed: true
        collapsed: true
        actions: [
            Kirigami.Action {
                text: i18n("Items")
                tooltip: i18n("Items in Katalogue")
                icon.name: "file-catalog-symbolic"
                onTriggered: {
                    pageStack.clear();
                    pageStack.push('qrc:Items.qml');
                }
            },
            Kirigami.Action {
                text: i18n("Notes")
                tooltip: i18n("View All Notes")
                icon.name: "backgroundtool-symbolic"
                onTriggered: {
                    pageStack.clear();
                    NoteModel.setItemId("");
                    pageStack.push('qrc:Notes.qml');
                }
            },
            Kirigami.Action {
                text: i18n("Documents")
                tooltip: i18n("View All Documents")
                icon.name: "folder-documents-symbolic"
                onTriggered: {
                    pageStack.clear();
                    DocumentModel.setItemId("");
                    pageStack.push('qrc:Documents.qml');
                }
            },
            Kirigami.Action {
                text: i18n("Tasks")
                tooltip: i18n("View All Tasks")
                icon.name: "view-task"
                onTriggered: {
                    TaskModel.resetItemIdQuery();
                    pageStack.clear();
                    pageStack.push('qrc:Tasks.qml');
                }
            },
            Kirigami.Action {
                text: i18n("Archive")
                tooltip: i18n("View Archived Items")
                icon.name: "folder-tar-symbolic"
                onTriggered: {
                    pageStack.clear();
                    pageStack.push("qrc:Items.qml", {
                        "itemModelType": ItemModel.ARCHIVE
                    });
                }
            },
            Kirigami.Action {
                text: i18n("About")
                tooltip: i18n("About Katalogue")
                icon.name: "help-about"
                onTriggered: pageStack.layers.push('qrc:About.qml')
            }
        ]

        header: RowLayout {
            Controls.ToolButton {
                icon.name: "application-menu"
                visible: globalDrawer.collapsible
                checked: !globalDrawer.collapsed
                onClicked: globalDrawer.collapsed = !globalDrawer.collapsed
            }

        }

    }

}
