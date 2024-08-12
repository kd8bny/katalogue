// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import com.kd8bny.katalogue
import com.kd8bny.katalogue.entries
import org.kde.kirigami as Kirigami

Kirigami.ScrollablePage {
    // itemParentBox.find(entryItem.itemId); //TODO wron item

    id: addEditItemPage

    property EntryItem entryItem
    property bool isEdit: false

    function insertUpdate() {
        entryItem.name = nameField.text;
        entryItem.make = makeField.text;
        entryItem.model = modelField.text;
        entryItem.year = yearField.text;
        if (typeBox.find(typeBox.editText) === -1)
            entryItem.type = typeBox.editText;
        else
            entryItem.type = typeBox.currentText;
        if (itemArchived.checked)
            entryItem.archived = true;

        if (itemParentEnabled.checked)
            entryItem.type.itemId = ItemParentModel.getId(itemParentBox.currentIndex);

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
            yearField.text = entryItem.year;
            typeBox.currentIndex = typeBox.find(entryItem.type);
            if (entryItem.archived)
                itemArchived.checked = entryItem.archived;

            if (entryItem.itemId)
                itemParentEnabled.checked = entryItem.itemId;

        } else {
            entryItem = ItemDatabase.getNewEntry();
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
                if (insertUpdate()) {
                    ItemModel.refresh();
                    ItemTypeModel.refresh();
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
        text: (isEdit) ? i18n("Failed to update Item") : i18n("Failed to insert Item")
        visible: false
    }

    Kirigami.InlineMessage {
        id: msgDeleteError

        type: Kirigami.MessageType.Error
        text: "Failed to delete Item"
        visible: false
    }

}
