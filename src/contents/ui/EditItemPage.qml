// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import com.kd8bny.katalogue 1.0

Kirigami.ScrollablePage {
    id: addItemPage

    required property string item_id
    property var itemData

    header: Kirigami.Heading {
        text: i18nc("@title:window", "Add new item to katalogue")
    }

    Kirigami.PromptDialog {
        id: deleteDialog

        title: i18n("Delete")
        subtitle: i18n("Are you sure you want to delete: ")// + itemName
        standardButtons: Kirigami.Dialog.Ok | Kirigami.Dialog.Cancel
        onAccepted: {
            Database.deleteItemEntry(item_id)
            ItemModel.updateModel()
            pageStack.clear()
            pageStack.push("qrc:Items.qml")
        }
        onRejected: {
            pageStack.pop("qrc:Items.qml")
        }
    }

    Component.onCompleted: {
        itemData = ItemModel.getItemData(item_id)
        console.log(itemData)
        nameField.text = itemData[0]
        makeField.text = itemData[1]
        modelField.text = itemData[2]
        yearField.text = itemData[3]
        archivedBox.checked = itemData[6]
    }

    Kirigami.FormLayout {
        id: form

        Controls.TextField {
            id: nameField
            Kirigami.FormData.label: i18nc("@label:textbox", "Name:")
            // placeholderText: i18n(itemName)
        }
        Controls.TextField {
            id: makeField
            Kirigami.FormData.label: i18nc("@label:textbox", "Make:")
            // placeholderText: i18n(itemMake)
        }
        Controls.TextField {
            id: modelField
            Kirigami.FormData.label: i18nc("@label:textbox", "Model:")
            // placeholderText: i18n(itemModel)
        }
        Controls.TextField {
            id: yearField
            Kirigami.FormData.label: i18nc("@label:textbox", "Year:")
            inputMask: "9999"
            // placeholderText: i18n(itemYear)
        }
        // Controls.ComboBox {
        //     editable: true
        //     Kirigami.FormData.label: i18nc("@label:textbox", "Category:")
        //     model: ListModel {
        //         id: categoryField
        //         ListElement { text: "Auto_ICE" }
        //         ListElement { text: "Utility" }
        //     }
        //     onAccepted: {
        //         if (find(editText) === -1)
        //             model.append({text: editText})
        //     }
        // }
        // Controls.ComboBox {
        //     editable: true
        //     Kirigami.FormData.label: i18nc("@label:textbox", "Group:")
        //     model: ListModel {
        //         id: groupField
        //         ListElement { text: "Default" }
        //     }
        //     onAccepted: {
        //         if (find(editText) === -1)
        //             model.append({text: editText})
        //     }
        // }
        Controls.CheckBox {
            id: archivedBox
            Kirigami.FormData.label: i18nc("@label:textbox", "Archive:")
        }
        Controls.Button {
            id: doneButton
            Layout.fillWidth: true
            text: i18nc("@action:button", "Update")
            enabled: nameField.text.length > 0
            onClicked: {
                Database.insertItemEntry(
                    nameField.text,
                    makeField.text,
                    modelField.text,
                    yearField.text,
                    "default",  // categoryField.text,
                    "default",  // groupField.text,
                    archivedBox.checked
                )
                ItemModel.updateModel()
                pageStack.pop()
            }
        }
        Controls.Button {
            id: deleteButton
            Layout.fillWidth: true
            text: i18nc("@action:button", "Delete")
            onClicked: deleteDialog.open()
        }
    }
}
