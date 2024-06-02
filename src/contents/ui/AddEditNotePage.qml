// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

import com.kd8bny.katalogue

Kirigami.ScrollablePage {
    id: addEditNotePage

    property int itemId: -1
    property int noteModelIndex: -1
    property bool isEdit: false

    title: (noteModelIndex != -1) ? i18n("Edit Note") : i18n("Add Note")

    actions: [
        Kirigami.Action {
            visible: isEdit
            text: i18n("Delete")
            icon.name: "delete"
            tooltip: i18n("Remove note")
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
            var success = NoteModel.deleteRecord(NoteModel.getId(noteModelIndex))
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
        var success = NoteModel.setRecord(
            noteModelIndex,
            titleField.text,
            noteContentField.text,
            itemId)

        if (success) {
            NoteModel.refresh()
        }

        return success
    }

    Component.onCompleted: {
        if (isEdit) {
            var recordData = NoteModel.getRecordAsList(noteModelIndex)

            console.log(recordData)
            titleField.text = recordData[3]
            noteContentField.text = recordData[4]
        }
        console.log(itemId)
    }

    Kirigami.FormLayout {
        Controls.TextField {
            id: titleField
            Kirigami.FormData.label: i18nc("@label:textbox", "Title:")
        }
        Controls.TextArea {
            id: noteContentField
            Layout.fillWidth: true  //TODO scrollview entry
            Kirigami.FormData.label: i18nc("@label:textbox", "Note:")
        }
        Controls.Button {
            id: doneButton
            Layout.fillWidth: true
            text: (noteModelIndex != -1) ? i18nc("@action:button", "Update") : i18nc("@action:button", "Add")
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
        text: "Failed to update Note"
        visible: false
    }

    Kirigami.InlineMessage {
        id: msgDeleteError
        type: Kirigami.MessageType.Error
        text: "Failed to delete Note"
        visible: false
    }
}
