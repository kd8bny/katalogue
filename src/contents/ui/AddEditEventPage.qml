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
    property string eventIndex: ""

    property var categories: Database.getEventCategories()
    property var types: Database.getItemTypes()

    title: (isEdit) ? i18n("Edit Event") : i18n("Add Event")

    actions {
        main: Kirigami.Action {
            enabled: isEdit
            text: i18n("Delete")
            icon.name: "delete"
            tooltip: i18n("Remove Attribute")
            onTriggered: {
                deleteDialog.open()
            }
        }
    }

    Kirigami.PromptDialog {
        id: deleteDialog

        title: i18n("Delete")
        subtitle: i18n("Are you sure you want to delete: ")
        standardButtons: Kirigami.Dialog.Ok | Kirigami.Dialog.Cancel
        onAccepted: {
            Database.deleteEventEntry(EventModel.getId(eventIndex))
            EventModel.updateModel()
            pageStack.pop()
        }
        onRejected: {
            pageStack.pop()
        }
    }

    Component.onCompleted: {
        var locale = Qt.locale()
        var currentDate = new Date()
        var dateString = currentDate.toLocaleDateString(locale, Locale.ShortFormat);
        dateField.text = dateString

        if (isEdit) {
            var recordData = EventModel.getRecord(eventIndex)
            dateField.text = recordData[1]
            eventField.text = recordData[2]
            costField.text = recordData[3]
            typeBox.find(recordData[4])
            categoryBox.find(recordData[5])
            commentField.text = recordData[6]
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
            id: commentField
            Kirigami.FormData.label: i18nc("@label:textbox", "Comment:")
        }
        Controls.ComboBox {
            id: categoryBox
            Kirigami.FormData.label: i18nc("@label:textbox", "Category")
            editable: true
            model: categories
        }
        Controls.ComboBox {
            id: typeBox
            Kirigami.FormData.label: i18nc("@label:textbox", "Item Type")
            model: types
        }
        Controls.Button {
            id: doneButton
            Layout.fillWidth: true
            text: (isEdit) ? i18nc("@action:button", "Update") : i18nc("@action:button", "Add")
            // enabled: (keyField.text.length & valueField.text.length) > 0
            onClicked: {
                var category = ""
                if (categoryBox.find(categoryBox.editText) === -1){
                    category = categoryBox.editText
                }else{
                    category = categoryBox.currentText
                }

                if(isEdit){
                    Database.updateEventEntry(
                        EventModel.getId(eventIndex),
                        dateField.text,
                        eventField.text,
                        costField.text,
                        category,
                        typeBox.currentText,
                        commentField.text
                    )
                }
                else{
                    Database.insertEventEntry(
                        itemId,
                        dateField.text,
                        eventField.text,
                        costField.text,
                        category,
                        typeBox.currentText,
                        commentField.text
                    )
                }

                EventModel.updateModel()
                EventModel.setItemId(itemId)
                pageStack.pop()
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
}
