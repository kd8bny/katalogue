// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami
import com.kd8bny.katalogue 1.0


Kirigami.ScrollablePage {
    id: page

    // required property string itemId
    // required property string itemName

    title: i18n("Notes")

    // actions { //TODO
    //     main: Kirigami.Action {
    //         text: i18n("Add")
    //         icon.name: "list-add"
    //         tooltip: i18n("Add new attribute")
    //         onTriggered: {
    //             pageStack.push("qrc:AddEditAttributePage.qml", {"itemId": itemId})
    //         }
    //     }
    // }

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
        delegate: noteDelegate
        // section.property: "category"
        // section.delegate: sectionDelegate
        // focus: true

        headerPositioning: ListView.OverlayHeader
        header: itemName

        Kirigami.PlaceholderMessage {
            anchors.centerIn: layout
            width: layout.width - (Kirigami.Units.largeSpacing * 4)

            visible: layout.count == 0
            text: i18n("Select add to add a general Note")
        }
    }

    Component {
        id: noteDelegate

        Kirigami.AbstractCard {
            contentItem: Item {
                implicitWidth: delegateLayout.implicitWidth
                implicitHeight: delegateLayout.implicitHeight
                GridLayout {
                    id: delegateLayout
                }
            }
        }
    }
}
