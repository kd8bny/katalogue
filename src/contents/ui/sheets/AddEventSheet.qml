// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import com.kd8bny.katalogue 1.0

Kirigami.OverlaySheet {
	id: addEventSheet

    required property string item_id

    parent: applicationWindow().overlay

    header: Kirigami.Heading {
        text: i18nc("@title:window", "Item Attribute")
    }

    Kirigami.FormLayout {
        Controls.TextField {
            id: dateField
            Kirigami.FormData.label: i18nc("@label:textbox", "Date:")
        }
        Controls.TextField {
            id: eventField
            Kirigami.FormData.label: i18nc("@label:textbox", "Event:")
        }
        Controls.TextField {
            id: costField
            Kirigami.FormData.label: i18nc("@label:textbox", "Cost:")
        }
        // Controls.TextField {
        //     id: valueField
        //     Kirigami.FormData.label: i18nc("@label:textbox", "Type:")
        // }
        //cat
        Controls.TextField {
            id: commentField
            Kirigami.FormData.label: i18nc("@label:textbox", "Comment:")
        }

        Controls.ComboBox {
            id: labelComboBox

            // editable: true
            Kirigami.FormData.label: i18nc("@label:textbox", ":")
            model: ListModel {
                id: labelField
                ListElement { text: "Auto_ICE" }
                ListElement { text: "Utility" }
            }
            onAccepted: {
                if (find(editText) === -1)
                    model.append({text: editText})
            }
        }
        Controls.Button {
            id: doneButton
            Layout.fillWidth: true
            text: i18nc("@action:button", "Add")
            //enabled: (keyField.text.length & valueField.text.length) > 0
            onClicked: {
                Database.insertEventEntry(
                    item_id,
                    dateField.text,
                    eventField.text,
                    costField.txt,
                    "type", "category",
                    commentField.text,
                )
                EventModel.setItemID(item_id)
                addEventSheet.close()
            }
        }
    }
}
