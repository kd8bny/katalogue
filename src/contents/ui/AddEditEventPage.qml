// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

import com.kd8bny.katalogue

Kirigami.ScrollablePage {
    id: addEditEventPage

    required property int itemId
    property int eventModelIndex: -1
    property bool isEdit: false

    title: (eventModelIndex != -1) ? i18n("Edit Event") : i18n("Add Event")

    actions: [
        Kirigami.Action {
            visible: isEdit
            text: i18n("Delete")
            icon.name: "delete"
            tooltip: i18n("Remove Attribute")
            onTriggered: {
                deleteDialog.open()
            }
        }
    ]

    Kirigami.PromptDialog {
        id: deleteDialog

        title: i18n("Delete")
        subtitle: i18n("Are you sure you want to delete: ")
        standardButtons: Kirigami.Dialog.Ok | Kirigami.Dialog.Cancel
        onAccepted: {
            var success = EventModel.deleteRecord(EventModel.getId(eventModelIndex))
            if (success) {
                pageStack.pop()
            } else {
                msgDeleteError.visible = true
            }}
        onRejected: {
            pageStack.pop()
        }
    }

    function insertUpdate() {
        var category = ""
        if (categoryBox.find(categoryBox.editText) === -1){
            category = categoryBox.editText
        }else{
            category = categoryBox.currentText
        }

        var success = EventModel.setRecord(
            eventModelIndex,
            dateField.text,
            eventField.text,
            parseFloat(costField.text),
            parseFloat(odometerField.text),
            category,
            commentField.text,
            itemId
        )

        if (success) {
            EventModel.refresh()
        }

        return success
    }

    Component.onCompleted: {
        var locale = Qt.locale()
        var currentDate = new Date()
        var dateString = currentDate.toLocaleDateString(locale, Locale.ShortFormat);
        dateField.text = Qt.formatDateTime(currentDate, Qt.ISODate).split("T")[0]

        if (isEdit) {
            var recordData = EventModel.getRecordAsList(eventModelIndex)

            dateField.text = recordData[3]
            eventField.text = recordData[4]
            costField.text = `${recordData[5]}`
            odometerField.text = `${recordData[6]}`
            categoryBox.currentIndex = categoryBox.find(recordData[7])
            commentField.text = recordData[8]
        }
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
        Controls.TextField {
            id: odometerField
            Kirigami.FormData.label: i18nc("@label:textbox", "Odometer:")
        }
        Controls.ComboBox {
            id: categoryBox
            Kirigami.FormData.label: i18nc("@label:textbox", "Category")
            editable: true
            model: EventCategoryModel
        }
        Controls.TextArea {
            id: commentField
            Layout.fillWidth: true
            Kirigami.FormData.label: i18nc("@label:textbox", "Comment")
        }
        Controls.Button {
            id: doneButton
            Layout.fillWidth: true
            text: (eventModelIndex != -1) ? i18nc("@action:button", "Update") : i18nc("@action:button", "Add")
            enabled: eventField.text.length > 0 //TODO check with not null
            onClicked: {
                var success = insertUpdate()

                if (success) {
                    pageStack.pop()
                }else {
                    msgInsertUpdateError.visible = true
                }
            }
        }
        Controls.Button {
            id: cancelButton
            Layout.fillWidth: true
            text: i18nc("@action:button", "Cancel")
            onClicked: {
                pageStack.pop()
            }
        }
    }

    Kirigami.InlineMessage {
        id: msgInsertUpdateError
        type: Kirigami.MessageType.Error
        text: "Failed to update Attribute"
        visible: false
    }

    Kirigami.InlineMessage {
        id: msgDeleteError
        type: Kirigami.MessageType.Error
        text: "Failed to delete Attribute"
        visible: false
    }
}
