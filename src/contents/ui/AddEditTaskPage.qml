// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import com.kd8bny.katalogue
import com.kd8bny.katalogue.entries
import org.kde.kirigami as Kirigami

Kirigami.ScrollablePage {
    id: addEditTaskPage

    property EntryItem entryItem
    property EntryTask entryTask
    property bool isEdit: false

    function insertUpdate() {
        entryTask.dueDate = dateField.text;
        entryTask.status = statusBox.currentText;
        entryTask.title = titleField.text;
        entryTask.description = descriptionField.text;
        if (entryItem)
            entryTask.itemId = entryItem.id;

        if (isEdit)
            return TaskDatabase.updateEntry(entryTask);
        else
            return TaskDatabase.insertEntry(entryTask);
    }

    title: (isEdit) ? i18n("Edit Task") : i18n("Add Task")
    Component.onCompleted: {
        if (entryTask) {
            isEdit = true;
            dateField.text = entryTask.dueDate;
            statusBox.currentIndex = statusBox.find(entryTask.status);
            titleField.text = entryTask.title;
            descriptionField.text = entryTask.description;
        } else {
            entryTask = TaskDatabase.getEntryById();
            var locale = Qt.locale();
            var currentDate = new Date();
            dateField.text = currentDate.toLocaleDateString(locale, Locale.ShortFormat).split("T")[0];
        }
    }
    actions: [
        Kirigami.Action {
            visible: isEdit
            text: i18n("Delete")
            icon.name: "delete"
            tooltip: i18n("Remove task")
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
            if (TaskDatabase.deleteEntryById(entryTask.id)) {
                TaskModel.onModelQueryChanged();
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
            id: dateField

            Kirigami.FormData.label: i18nc("@label:textbox", "Date:")
        }

        Controls.ComboBox {
            id: statusBox

            Kirigami.FormData.label: i18nc("@label:textbox", "Status:")
            model: Defaults.getValidTaskStatus()
        }

        Controls.TextField {
            id: titleField

            Kirigami.FormData.label: i18nc("@label:textbox", "Title:")
        }

        Controls.TextArea {
            id: descriptionField

            Layout.fillWidth: true //TODO scrollview entry
            Kirigami.FormData.label: i18nc("@label:textbox", "Description:")
        }

        Controls.Button {
            id: doneButton

            Layout.fillWidth: true
            text: (isEdit) ? i18nc("@action:button", "Update") : i18nc("@action:button", "Add")
            onClicked: {
                if (insertUpdate()) {
                    TaskModel.onModelQueryChanged();
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
        text: (isEdit) ? i18n("Failed to update Task") : i18n("Failed to insert Task")
        visible: false
    }

    Kirigami.InlineMessage {
        id: msgDeleteError

        type: Kirigami.MessageType.Error
        text: "Failed to delete Task"
        visible: false
    }

}
