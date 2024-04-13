// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import com.kd8bny.katalogue


Kirigami.ScrollablePage {
    id: page

    required property string itemId
    required property string itemName

    title: i18n("Notes")

    actions: [
        Kirigami.Action {
            text: i18n("Add Note")
            icon.name: "list-add"
            tooltip: i18n("Add new note")
            onTriggered: {
                pageStack.push("qrc:AddEditNotePage.qml")
            }
        }
    ]

    Component.onCompleted: {
        NoteModel.setItemId(itemId)
    }

    Kirigami.CardsListView {
        id: layout
        model: NoteModel
        delegate: notesDelegate

        headerPositioning: ListView.OverlayHeader

        Kirigami.PlaceholderMessage {
            anchors.centerIn: layout
            width: layout.width - (Kirigami.Units.largeSpacing * 4)

            visible: layout.count == 0
            text: i18n("Select 'Add Note' to insert a new Note")
        }
    }

    Component {
        id: notesDelegate

        Kirigami.Card {
            showClickFeedback: true

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
                    anchors {
                        left: parent.left
                        top: parent.top
                        right: parent.right
                        //IMPORTANT: never put the bottom margin
                    }
                    rowSpacing: Kirigami.Units.largeSpacing
                    columnSpacing: Kirigami.Units.largeSpacing
                    columns: width > Kirigami.Units.gridUnit * 20 ? 4 : 2
                    Controls.Label {
                        wrapMode: TextEdit.Wrap
                        text: note
                    }
                }
            }
        }
    }
}
