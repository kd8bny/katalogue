// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import com.kd8bny.katalogue 1.0

Kirigami.ScrollablePage {
    id: addEditAttributePage

    required property string itemId
    property int attributeIndex: -1

    title: (attributeIndex != -1) ? i18n("Edit Attribute") : i18n("Add Attribute")

    actions {
        main: Kirigami.Action {
            enabled: attributeIndex != -1
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
            AttributeModel.deleteRecord(AttributeModel.getId(attributeIndex))
            pageStack.pop()
        }
        onRejected: {
            pageStack.pop()
        }
    }

    Component.onCompleted: {
        if (attributeIndex != -1) {
            var recordData = AttributeModel.getRecordAsList(attributeIndex)
            keyField.text = recordData[1]
            valueField.text = recordData[2]
            categoryBox.find(recordData[3])
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
        Controls.Button {
            id: doneButton
            Layout.fillWidth: true
            text: (attributeIndex != -1) ? i18nc("@action:button", "Update") : i18nc("@action:button", "Add")
            enabled: (keyField.text.length && valueField.text.length) > 0
            onClicked: {
                var category = ""
                if (categoryBox.find(categoryBox.editText) === -1){
                    category = categoryBox.editText
                }else{
                    category = categoryBox.currentText
                }

                AttributeModel.setRecord(
                    attributeIndex,
                    keyField.text,
                    valueField.text,
                    category,
                    itemId
                )

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
