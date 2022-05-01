// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami

Kirigami.ApplicationWindow {
    id: root

    title: i18n("katalogue")

    minimumWidth: Kirigami.Units.gridUnit * 20
    minimumHeight: Kirigami.Units.gridUnit * 20

    onClosing: Controller.saveWindowGeometry(root)

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
        onTriggered: Controller.saveWindowGeometry(root)
    }

    globalDrawer: Kirigami.GlobalDrawer {
        id: globalDrawer
        title: i18n("katalogue")
        titleIcon: "applications-graphics"
        isMenu: !root.isMobile
        modal: false;
        collapsible: true;
        showHeaderWhenCollapsed: true

        header: RowLayout {
            Layout.fillWidth: true
            Controls.ToolButton {
                icon.name: "application-menu"
                visible: globalDrawer.collapsible
                checked: !globalDrawer.collapsed
                onClicked: globalDrawer.collapsed = !globalDrawer.collapsed
            }
            Kirigami.SearchField {
                visible: !globalDrawer.collapsed
                Layout.fillWidth: true
            }
        }

        actions: [
            Kirigami.Action {
                text: i18n("Todo")
                icon.name: "task-new"
                // onTriggered: {
                //     addSheet.open()
                // }
            },
            Kirigami.Action {
                text: i18n("Log")
                icon.name: "file-catalog-symbolic"
                // onTriggered: {
                //     addSheet.open()
                // }
            },
            Kirigami.Action {
                text: i18n("About katalogue")
                icon.name: "help-about"
                onTriggered: pageStack.layers.push('qrc:About.qml')
            },
            Kirigami.Action {
                text: i18n("Quit")
                icon.name: "application-exit"
                onTriggered: Qt.quit()
            }
        ]
    }

    contextDrawer: Kirigami.ContextDrawer {
        id: contextDrawer
    }

    Overview{
        id: overviewPage
    }

    pageStack.initialPage: overviewPage


}
