// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import com.kd8bny.katalogue
import com.kd8bny.katalogue.entries
import org.kde.kirigami as Kirigami

Kirigami.ScrollablePage {
    id: addEditItemPage

    property EntryItem entryItem
    property bool isEdit: false

    signal modelModified()

    function insertUpdate() {
        entryItem.name = nameField.text;
        entryItem.make = makeField.text;
        entryItem.model = modelField.text;
        entryItem.year = yearField.value;
        if (typeBox.find(typeBox.editText) === -1)
            entryItem.type = typeBox.editText;
        else
            entryItem.type = typeBox.currentText;
        if (itemArchived.checked)
            entryItem.archived = true;

        if (itemParentEnabled.checked)
            entryItem.itemId = ItemParentModel.getId(itemParentBox.currentIndex);

        if (isEdit)
            return ItemDatabase.updateEntry(entryItem);
        else
            return ItemDatabase.insertEntry(entryItem);
    }

    title: (isEdit) ? i18n("Edit Item") : i18n("Add Item")
    Component.onCompleted: {
        if (entryItem) {
            isEdit = true;
            nameField.text = entryItem.name;
            makeField.text = entryItem.make;
            modelField.text = entryItem.model;
            yearField.value = entryItem.year;
            typeBox.currentIndex = typeBox.find(entryItem.type);
            if (entryItem.archived)
                itemArchived.checked = entryItem.archived;

            if (entryItem.itemId) {
                itemParentEnabled.checked = entryItem.itemId;
                var parentEntry = ItemDatabase.getEntryById(entryItem.itemId);
                itemParentBox.currentIndex = itemParentBox.find(parentEntry.name);
            }
        } else {
            entryItem = ItemDatabase.getEntryById();
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

    Controls.Action {
        id: addUpdateAction

        enabled: nameField.text.length > 0
        shortcut: "Return"
        onTriggered: {
            if (insertUpdate()) {
                ItemModel.refresh();
                ItemTypeModel.refresh();
                pageStack.pop();
            } else {
                msgInsertUpdateError.visible = true;
            }
        }
    }

    Kirigami.PromptDialog {
        id: deleteDialog

        title: i18n("Delete")
        subtitle: i18n("Are you sure you want to delete Item")
        standardButtons: Kirigami.Dialog.Ok | Kirigami.Dialog.Cancel
        onAccepted: {
            if (ItemDatabase.deleteEntryById(entryItem.id)) {
                ItemModel.refresh();
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
        // RowLayout {
        // Layout.fillWidth: true
        // }

        id: form

        Controls.TextField {
            id: nameField

            Kirigami.FormData.label: i18nc("@label:textbox", "Name:")
            placeholderText: i18n("Required")
        }

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
        }

        Controls.TextField {
            id: makeField

            Kirigami.FormData.label: i18nc("@label:textbox", "Make:")
        }

        Controls.TextField {
            id: modelField

            Kirigami.FormData.label: i18nc("@label:textbox", "Model:")
        }

        Controls.SpinBox {
            id: yearField

            Layout.fillWidth: true
            Kirigami.FormData.label: i18nc("@label:textbox", "Year:")
            from: 1900
            to: 3000
            value: 2000
            textFromValue: function(year) {
                // Do not format the year
                return year;
            }
        }

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
        }

        Controls.ComboBox {
            id: typeBox

            Layout.fillWidth: true
            Kirigami.FormData.label: i18nc("@label:textbox", "Type:")
            editable: true
            model: ItemTypeModel
        }

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
        }

        Controls.Switch {
            id: itemParentEnabled

            Kirigami.FormData.label: i18n("Item Component:")
        }

        Controls.ComboBox {
            id: itemParentBox

            Layout.fillWidth: true
            Kirigami.FormData.label: i18nc("@label:textbox", "Component Parent:")
            editable: false
            visible: itemParentEnabled.checked
            model: ItemParentModel
        }

        Controls.Switch {
            id: itemArchived

            enabled: isEdit
            visible: isEdit
            Kirigami.FormData.label: i18nc("@label:textbox", i18n("Archive"))
        }

    }

    Kirigami.InlineMessage {
        id: msgInsertUpdateError

        type: Kirigami.MessageType.Error
        text: (isEdit) ? i18n("Failed to update Item") : i18n("Failed to insert Item")
        visible: false
    }

    Kirigami.InlineMessage {
        id: msgDeleteError

        type: Kirigami.MessageType.Error
        text: "Failed to delete Item"
        visible: false
    }

    footer: Controls.DialogButtonBox {
        standardButtons: Controls.DialogButtonBox.Cancel
        onRejected: pageStack.pop()
        onAccepted: addUpdateAction.trigger()

        Controls.Button {
            icon.name: isEdit ? "document-save" : "list-add"
            text: isEdit ? i18n("Save") : i18n("Add")
            Controls.DialogButtonBox.buttonRole: Controls.DialogButtonBox.AcceptRole
            enabled: nameField.length > 0
        }

    }

}
