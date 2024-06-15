// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import com.kd8bny.katalogue
import org.kde.kirigami as Kirigami

Kirigami.ScrollablePage {
    id: addEditItemPage

    property int itemModelIndex: -1
    property bool isEdit: false
    property bool isArchived: false

    function insertUpdate() {
        var typeText = "";
        if (typeBox.find(typeBox.editText) === -1)
            typeText = typeBox.editText;
        else
            typeText = typeBox.currentText;
        var archived = false;
        if (itemArchived.checked)
            archived = true;

        var parentId = -1;
        if (itemParentEnabled.checked)
            parentId = ItemParentModel.getId(itemParentBox.currentIndex);

        var success = ItemModel.setRecord(itemModelIndex, nameField.text, makeField.text, modelField.text, parseInt(yearField.text), typeText, archived, parentId);
        if (success)
            ItemTypeModel.refresh();

        return success;
    }

    title: (isEdit) ? i18n("Edit Item") : i18n("Add Item")
    Component.onCompleted: {
        if (isEdit) {
            var recordData = ItemModel.getRecordAsList(itemModelIndex);
            console.log(recordData);
            nameField.text = recordData[3];
            makeField.text = recordData[4];
            modelField.text = recordData[5];
            yearField.text = recordData[6];
            typeBox.currentIndex = typeBox.find(recordData[7]);
            if (recordData[8]) {
                itemArchived.checked = recordData[8];
                isArchived = true;
            }
            if (recordData[10]) {
                itemParentEnabled.checked = recordData[6];
                itemParentBox.find(recordData[10]);
            }
        }
    }
    actions: [
        Kirigami.Action {
            visible: isEdit
            text: i18n("Delete")
            icon.name: "delete"
            tooltip: i18n("Remove item from Katalogue")
            onTriggered: {
                deleteDialog.open();
            }
        }
    ]

    Kirigami.PromptDialog {
        id: deleteDialog

        title: i18n("Delete")
        subtitle: i18n("Are you sure you want to delete Item")
        standardButtons: Kirigami.Dialog.Ok | Kirigami.Dialog.Cancel
        onAccepted: {
            var success = ItemModel.deleteRecord(ItemModel.getId(itemModelIndex));
            if (success) {
                pageStack.clear();
                pageStack.push("qrc:Items.qml");
            } else {
                msgDeleteError.visible = true;
            }
        }
        onRejected: {
            pageStack.pop();
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
            text: "2000"
        }

        Controls.ComboBox {
            id: typeBox

            Kirigami.FormData.label: i18nc("@label:textbox", "Type:")
            editable: true
            model: ItemTypeModel
        }

        Controls.Switch {
            id: itemParentEnabled

            Kirigami.FormData.label: i18nc("@label:textbox", "Item is a component")
        }

        Controls.ComboBox {
            id: itemParentBox

            editable: false
            visible: itemParentEnabled.checked
            model: ItemParentModel
            Kirigami.FormData.label: i18nc("@label:textbox", "Component of:")
        }

        Controls.Switch {
            id: itemArchived

            enabled: isEdit
            visible: isEdit
            Kirigami.FormData.label: i18nc("@label:textbox", i18n("Archive"))
        }

        Controls.Button {
            id: doneButton

            Layout.fillWidth: true
            text: (isEdit) ? i18nc("@action:button", "Update") : i18nc("@action:button", "Add")
            enabled: nameField.text.length > 0
            onClicked: {
                var success = insertUpdate();
                if (success)
                    pageStack.pop();
                else
                    msgInsertUpdateError.visible = true;
            }
        }

        Controls.Button {
            id: cancelButton

            Layout.fillWidth: true
            text: i18nc("@action:button", "Cancel")
            onClicked: {
                pageStack.pop();
            }
        }

    }

    Kirigami.InlineMessage {
        id: msgInsertUpdateError

        type: Kirigami.MessageType.Error
        text: "Failed to update Item"
        visible: false
    }

    Kirigami.InlineMessage {
        id: msgDeleteError

        type: Kirigami.MessageType.Error
        text: "Failed to delete Item"
        visible: false
    }

}
