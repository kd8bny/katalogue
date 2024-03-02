// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import com.kd8bny.katalogue 1.0
import org.kde.kirigami 2.20 as Kirigami


Kirigami.ScrollablePage {
    id: page

    // required property string itemId
    // required property string itemName

    title: i18n("Notes")

    actions { //TODO
        main: Kirigami.Action {
            text: i18n("Add Note")
            icon.name: "list-add"
            tooltip: i18n("Add new note")
            onTriggered: {
                pageStack.push("qrc:AddEditNotePage.qml")
            }
        }
    }

    // function openInfoSheet(index = -1) {
    //     var recordData = NoteModel.getRecordAsList(index)

    //     attributeInfoSheet.key = recordData[1]
    //     attributeInfoSheet.value = recordData[2]
    //     attributeInfoSheet.category = recordData[3]

    //     attributeInfoSheet.open()
    // }

    // Component.onCompleted {
    //     // NoteModel.setItemId(itemId)
    // }

    Kirigami.CardsListView {
        id: layout
        model: NoteModel
        delegate: notesDelegate
        // section.property: "category"
        // section.delegate: sectionDelegate
        // focus: true

        headerPositioning: ListView.OverlayHeader
        // header: itemName

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
            // onClicked:  {
            //     pageStack.push("qrc:Details.qml", {"itemModelIndex": index, "itemId": id, "itemName": name, "isComponentView": isComponentView})
            // }

            // TODO actions

            banner {
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
                    Controls.Text {
                        wrapMode: TextEdit.Wrap
                        text: note
                    }
                }
            }
        }
    }
}
