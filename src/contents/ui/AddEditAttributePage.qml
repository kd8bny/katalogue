// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import com.kd8bny.katalogue
import com.kd8bny.katalogue.entries
import org.kde.kirigami as Kirigami

Kirigami.ScrollablePage {
    id: addEditAttributePage

    required property EntryItem entryItem
    property EntryAttribute entryAttribute
    property bool isEdit: false

    function insertUpdate() {
        entryAttribute.key = keyField.text;
        entryAttribute.value = valueField.text;
        if (categoryBox.find(categoryBox.editText) === -1)
            entryAttribute.category = categoryBox.editText;
        else
            entryAttribute.category = categoryBox.currentText;
        entryAttribute.itemId = entryItem.id;
        if (isEdit)
            return AttributeDatabase.updateEntry(entryAttribute);
        else
            return AttributeDatabase.insertEntry(entryAttribute);
    }

    title: (isEdit) ? i18n("Edit Attribute") : i18n("Add Attribute")
    Component.onCompleted: {
        if (entryAttribute) {
            isEdit = true;
            keyField.text = entryAttribute.key;
            valueField.text = entryAttribute.value;
            categoryBox.currentIndex = categoryBox.find(entryAttribute.category);
        } else {
            entryAttribute = AttributeDatabase.getEntryById();
        }
    }
    actions: [
        Kirigami.Action {
            visible: isEdit
            text: i18n("Delete")
            icon.name: "delete"
            tooltip: i18n("Remove Attribute")
            onTriggered: {
                deleteDialog.open();
            }
        }
    ]

    Kirigami.PromptDialog {
        id: deleteDialog

        title: i18n("Delete")
        subtitle: i18n("Are you sure you want to delete: ")
        standardButtons: Kirigami.Dialog.Ok | Kirigami.Dialog.Cancel
        onAccepted: {
            if (AttributeDatabase.deleteEntryById(entryAttribute.id)) {
                AttributeModel.refreshModel();
                AttributeCategoryModel.refreshModel();
                pageStack.pop();
            } else {
                msgDeleteError.visible = true;
            }
        }
        onRejected: {
            pageStack.pop();
        }
    }

    Kirigami.FormLayout {
        Controls.TextField {
            id: keyField

            Kirigami.FormData.label: i18nc("@label:textbox", "Name:")
        }

        Controls.TextField {
            id: valueField

            Layout.fillWidth: true
            Kirigami.FormData.label: i18nc("@label:textbox", "Value:")
        }

        Controls.ComboBox {
            id: categoryBox

            Layout.fillWidth: true
            editable: true
            Kirigami.FormData.label: i18nc("@label:textbox", "Category:")
            model: UniqueValueModelFactory.createAttributeCategoryModel()
        }

        Controls.Button {
            id: doneButton

            Layout.fillWidth: true
            text: (isEdit) ? i18nc("@action:button", "Update") : i18nc("@action:button", "Add")
            enabled: (keyField.text.length && valueField.text.length) > 0
            onClicked: {
                if (insertUpdate()) {
                    AttributeModel.refreshModel();
                    AttributeCategoryModel.refreshModel();
                    pageStack.pop();
                } else {
                    msgInsertUpdateError.visible = true;
                }
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
        text: (isEdit) ? i18n("Failed to update Attribute") : i18n("Failed to insert Attribute")
        visible: false
    }

    Kirigami.InlineMessage {
        id: msgDeleteError

        type: Kirigami.MessageType.Error
        text: "Failed to delete Attribute"
        visible: false
    }

}
