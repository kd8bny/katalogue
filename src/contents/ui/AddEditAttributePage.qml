// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

import com.kd8bny.katalogue

Kirigami.ScrollablePage {
    id: addEditAttributePage

    required property string itemId
    property int attributeModelIndex: -1
    property bool isEdit: false

    title: (attributeModelIndex != -1) ? i18n("Edit Attribute") : i18n("Add Attribute")

    actions : [
        Kirigami.Action {
            enabled: attributeModelIndex != -1
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
            var success = AttributeModel.deleteRecord(AttributeModel.getId(attributeModelIndex))
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

        var success = AttributeModel.setRecord(
            attributeModelIndex,
            keyField.text,
            valueField.text,
            category,
            itemId
        )

        if (success) {
            AttributeModel.refresh()
        }

        return success
    }

    Component.onCompleted: {
        if (isEdit) {
            var recordData = AttributeModel.getRecordAsList(attributeModelIndex)

            keyField.text = recordData[3]
            valueField.text = recordData[4]
            categoryBox.currentIndex = categoryBox.find(recordData[5])
        }
    }

    Kirigami.FormLayout {
        Controls.TextField {
            id: keyField
            Kirigami.FormData.label: i18nc("@label:textbox", "Name:")
        }
        Controls.TextField {
            id: valueField
            Kirigami.FormData.label: i18nc("@label:textbox", "Value:")
        }
        Controls.ComboBox {
            id: categoryBox
            editable: true
            Kirigami.FormData.label: i18nc("@label:textbox", "Category:")
            model: AttributeCategoryModel
        }
        // TODO parent box to change
        Controls.Button {
            id: doneButton
            Layout.fillWidth: true
            text: (attributeModelIndex != -1) ? i18nc("@action:button", "Update") : i18nc("@action:button", "Add")
            enabled: (keyField.text.length && valueField.text.length) > 0
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
