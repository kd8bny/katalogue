// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami
import com.kd8bny.katalogue 1.0

Kirigami.ScrollablePage {
    id: addEditNotePage

    property int itemId: -1
    property int noteIndex: -1

    title: (noteIndex != -1) ? i18n("Edit Note") : i18n("Add Note")

    actions {
        main: Kirigami.Action {
            enabled: noteIndex != -1
            text: i18n("Delete")
            icon.name: "delete"
            tooltip: i18n("Remove note")
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
            NoteModel.deleteRecord(NoteModel.getId(noteIndex))
            pageStack.pop()
        }
        onRejected: {
            pageStack.pop()
        }
    }

    Component.onCompleted: {
        var locale = Qt.locale()
        var currentDate = new Date()
        var dateString = currentDate.toLocaleDateString(locale, Locale.ShortFormat);
        // dateField.text = dateString

        if (noteIndex != -1) {
            var recordData = NoteModel.getRecordAsList(noteIndex)

            // dateField.text = recordData[1]
            titleField.text = recordData[1]
            noteContentField.text = recordData[2]
        }
    }

    Kirigami.FormLayout {
        Controls.TextField {
            id: titleField
            Kirigami.FormData.label: i18nc("@label:textbox", "Title:")
        }
        Controls.TextArea {
            id: noteContentField
            Kirigami.FormData.label: i18nc("@label:textbox", "Note:")
        }
        Controls.Button {
            id: doneButton
            Layout.fillWidth: true
            text: (noteIndex != -1) ? i18nc("@action:button", "Update") : i18nc("@action:button", "Add")
            onClicked: {

                NoteModel.setRecord(
                    noteIndex,
                    titleField.text,
                    noteContentField.text,
                    itemId)

                //TODO check results of insert
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
