// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import com.kd8bny.katalogue 1.0

Kirigami.ScrollablePage {
    id: addEditItemPage

    property string itemId
    property bool isEdit: false

    property var types: ItemModel.getItemTypes()

    title: (isEdit) ? i18n("Edit Item") : i18n("Add Item")

    actions {
        main: Kirigami.Action {
            enabled: isEdit
            text: i18n("Delete")
            icon.name: "delete"
            tooltip: i18n("Remove Item")
            onTriggered: {
                deleteDialog.open()
            }
        }
    }

    Kirigami.PromptDialog {
        id: deleteDialog

        title: i18n("Delete")
        subtitle: i18n("Are you sure you want to delete Item")
        standardButtons: Kirigami.Dialog.Ok | Kirigami.Dialog.Cancel
        onAccepted: {
            Database.deleteItemEntry(itemId)
            ItemModel.updateModel()
            pageStack.clear()
            pageStack.push("qrc:Items.qml")
        }
        onRejected: {
            pageStack.pop()//"qrc:Items.qml")
        }
    }

    Kirigami.FormLayout {
        id: form

        Controls.TextField {
            id: nameField
            Kirigami.FormData.label: i18nc("@label:textbox", "Name:")
            placeholderText: i18n("Required")
        }
        Controls.TextField {
            id: makeField
            Kirigami.FormData.label: i18nc("@label:textbox", "Make:")
        }
        Controls.TextField {
            id: modelField
            Kirigami.FormData.label: i18nc("@label:textbox", "Model:")
        }
        Controls.TextField {
            id: yearField
            Kirigami.FormData.label: i18nc("@label:textbox", "Year:")
            placeholderText: i18n("YYYY")
            inputMask: "9999"
        }
        Controls.ComboBox {
            id: typeBox
            editable: true
            Kirigami.FormData.label: i18nc("@label:textbox", "Item Type:")
            model: types
        }

        Controls.CheckBox {
            id: itemParentEnabled
            Kirigami.FormData.label: i18nc("@label:textbox", "Component of Item:")
        }
        Controls.ComboBox {
            id: itemParentBox
            editable: false
            enabled: itemParentEnabled.checked
            model: ItemModel.getItemParents()
            Kirigami.FormData.label: i18nc("@label:textbox", "Component of:")
        }

        Controls.Button {
            id: doneButton
            Layout.fillWidth: true
            text: i18nc("@action:button", "Add")
            enabled: nameField.text.length > 0
            onClicked: {
                var type = ""
                if (typeBox.find(typeBox.editText) === -1){
                    type = typeBox.editText
                }else{
                    type = typeBox.currentText
                }

                var parentId = "NULL"
                if (itemParentEnabled.checked){
                    parentId = ItemModel.getId(itemParentBox.currentIndex)
                }

                Database.insertItemEntry(
                    nameField.text,
                    makeField.text,
                    modelField.text,
                    yearField.text,
                    type,
                    parentId
                )
                ItemModel.updateModel()
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
