// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import com.kd8bny.katalogue
import com.kd8bny.katalogue.entries
import org.kde.kirigami as Kirigami

Kirigami.ScrollablePage {
    id: taskPage

    property EntryItem entryItem

    function getEntryByIndex(index) {
        return TaskDatabase.getEntryById(TaskModel.getId(index));
    }

    title: (entryItem) ? i18n(entryItem.name + " Tasks") : i18n("All Tasks")
    Component.onCompleted: {
        if (entryItem)
            TaskModel.setItemIdQuery(entryItem.id);

    }
    actions: [
        Kirigami.Action {
            text: i18n("Add Task")
            icon.name: "list-add"
            tooltip: i18n("Add new task")
            onTriggered: {
                pageStack.push("qrc:AddEditTaskPage.qml", {
                    "entryItem": entryItem
                });
            }
        }
    ]

    Kirigami.CardsListView {
        id: layout

        model: TaskModel
        delegate: tasksDelegate
        headerPositioning: ListView.OverlayHeader

        Kirigami.PlaceholderMessage {
            anchors.centerIn: layout
            width: layout.width - (Kirigami.Units.largeSpacing * 4)
            visible: layout.count == 0
            text: i18n("Select 'Add Task' to insert a new Task")
        }

    }

    Component {
        id: tasksDelegate

        Kirigami.Card {
            showClickFeedback: true
            onClicked: {
                pageStack.push("qrc:AddEditTaskPage.qml", {
                    "entryItem": entryItem,
                    "entryTask": getEntryByIndex(index)
                });
            }

            banner {
                //TODO clean up view
                // source: "../banner.jpg"
                title: title
                // The title can be positioned in the banner
                titleAlignment: Qt.AlignLeft | Qt.AlignBottom
            }

            contentItem: Item {
                // implicitWidth/Height define the natural width/height of an item if no width or height is specified.
                // The setting below defines a component's preferred size based on its content
                implicitWidth: delegateLayout.implicitWidth
                implicitHeight: delegateLayout.implicitHeight

                GridLayout {
                    id: delegateLayout

                    rowSpacing: Kirigami.Units.largeSpacing
                    columnSpacing: Kirigami.Units.largeSpacing
                    columns: width > Kirigami.Units.gridUnit * 20 ? 4 : 2

                    anchors {
                        //IMPORTANT: never put the bottom margin

                        left: parent.left
                        top: parent.top
                        right: parent.right
                    }

                    Controls.Label {
                        wrapMode: TextEdit.Wrap
                        text: description
                    }

                    Controls.Label {
                        wrapMode: TextEdit.Wrap
                        text: status
                    }

                }

            }

        }

    }

}
