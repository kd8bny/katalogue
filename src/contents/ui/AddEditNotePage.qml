// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import com.kd8bny.katalogue
import com.kd8bny.katalogue.entries
import org.kde.kirigami as Kirigami

Kirigami.ScrollablePage {
    id: addEditNotePage

    property EntryItem entryItem
    property EntryNote entryNote
    property bool isEdit: false

    function insertUpdate() {
        entryNote.title = titleField.text;
        entryNote.noteContent = noteContentField.text;
        if (entryItem)
            entryNote.itemId = entryItem.id;

        if (isEdit)
            return NoteDatabase.updateEntry(entryNote);
        else
            return NoteDatabase.insertEntry(entryNote);
    }

    title: (isEdit) ? i18n("Edit Note") : i18n("Add Note")
    Component.onCompleted: {
        if (entryNote) {
            isEdit = true;
            titleField.text = entryNote.title;
            noteContentField.text = entryNote.noteContent;
        } else {
            entryNote = NoteDatabase.getEntryById();
        }
    }
    actions: [
        Kirigami.Action {
            visible: isEdit
            text: i18n("Delete")
            icon.name: "delete"
            tooltip: i18n("Remove note")
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
            if (NoteDatabase.deleteEntryById(entryNote.id)) {
                NoteModel.refresh();
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
            id: titleField

            Kirigami.FormData.label: i18nc("@label:textbox", "Title:")
        }

        Controls.TextArea {
            id: noteContentField

            Layout.fillWidth: true //TODO scrollview entry
            Kirigami.FormData.label: i18nc("@label:textbox", "Note:")
        }

        Controls.Button {
            id: doneButton

            Layout.fillWidth: true
            text: (isEdit) ? i18nc("@action:button", "Update") : i18nc("@action:button", "Add")
            onClicked: {
                if (insertUpdate()) {
                    NoteModel.refresh();
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
        text: (isEdit) ? i18n("Failed to update Note") : i18n("Failed to insert Note")
        visible: false
    }

    Kirigami.InlineMessage {
        id: msgDeleteError

        type: Kirigami.MessageType.Error
        text: "Failed to delete Note"
        visible: false
    }

}
