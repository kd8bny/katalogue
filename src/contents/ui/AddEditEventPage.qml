// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import com.kd8bny.katalogue 1.0

Kirigami.ScrollablePage {
    id: addEditEventPage

    required property string itemId
    property bool isEdit: false

    property string eventId: ""
    property string date: ""
    property string event: ""
    property string cost: ""
    property string type: ""
    property string category: ""
    property string comment: ""

    header: Kirigami.Heading {
        text: i18nc("@title:window", "Event")
    }

    Kirigami.PromptDialog {
        id: deleteDialog

        title: i18n("Delete")
        subtitle: i18n("Are you sure you want to delete: ")
        standardButtons: Kirigami.Dialog.Ok | Kirigami.Dialog.Cancel
        onAccepted: {
            Database.deleteItemEntry(itemId)
            ItemModel.updateModel()
            pageStack.clear()
            pageStack.push("qrc:Items.qml")
        }
        onRejected: {
            pageStack.pop("qrc:Items.qml")
        }
    }

    Kirigami.FormLayout {
        Controls.TextField {
            id: dateField
            text: date
            Kirigami.FormData.label: i18nc("@label:textbox", "Date:")
        }
        Controls.TextField {
            id: eventField
            text: event
            Kirigami.FormData.label: i18nc("@label:textbox", "Event:")
        }
        Controls.TextField {
            id: costField
            text: cost
            Kirigami.FormData.label: i18nc("@label:textbox", "Cost:")
        }
        // Controls.TextField {
        //     id: valueField
        //     Kirigami.FormData.label: i18nc("@label:textbox", "Type:")
        // }
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
            text: i18nc("@action:button", "Add") //TODO add/update
            // enabled: (keyField.text.length & valueField.text.length) > 0
            onClicked: {
                if(isEdit){
                    Database.updateEventEntry(
                        eventId,
                        dateField.text,
                        eventField.text,
                        costField.txt,
                        "type", "category",
                        commentField.text,
                    )
                }
                else{
                    Database.insertEventEntry(
                        itemId,
                        dateField.text,
                        eventField.text,
                        costField.txt,
                        "type", "category",
                        commentField.text,
                    )
                }

                EventModel.updateModel()
                pageStack.pop()
            }
        }
    }
}
