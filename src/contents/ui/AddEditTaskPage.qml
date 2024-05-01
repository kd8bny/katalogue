// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

import com.kd8bny.katalogue

Kirigami.ScrollablePage {
    id: addEditTaskPage

    property int itemId: -1
    property int taskModelIndex: -1
    property bool isEdit: false

    title: (taskModelIndex != -1) ? i18n("Edit Task") : i18n("Add Task")

    actions: [
        Kirigami.Action {
            visible: isEdit
            text: i18n("Delete")
            icon.name: "delete"
            tooltip: i18n("Remove task")
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
            var success = TaskModel.deleteRecord(TaskModel.getId(taskModelIndex))
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
        var success = TaskModel.setRecord(
            taskModelIndex,
            titleField.text,
            taskContentField.text,
            itemId)

        if (success) {
            TaskModel.refresh()
        }

        return success
    }

    Component.onCompleted: {
        var locale = Qt.locale()
        var currentDate = new Date()
        var dateString = currentDate.toLocaleDateString(locale, Locale.ShortFormat);

        if (isEdit) {
            var recordData = TaskModel.getRecordAsList(taskModelIndex)

            console.log(recordData)


            dateField.text = recordData[2]
            titleField.text = recordData[3]
            taskContentField.text = recordData[4]
        }
    }

    Kirigami.FormLayout {
        Controls.TextField {
            id: dateField
            Kirigami.FormData.label: i18nc("@label:textbox", "Date:")
        }
        Controls.TextField {
            id: titleField
            Kirigami.FormData.label: i18nc("@label:textbox", "Title:")
        }
        Controls.TextArea {
            id: taskContentField
            Layout.fillWidth: true  //TODO scrollview entry
            Kirigami.FormData.label: i18nc("@label:textbox", "Task:")
        }
        Controls.Button {
            id: doneButton
            Layout.fillWidth: true
            text: (taskModelIndex != -1) ? i18nc("@action:button", "Update") : i18nc("@action:button", "Add")
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
        text: "Failed to update Task"
        visible: false
    }

    Kirigami.InlineMessage {
        id: msgDeleteError
        type: Kirigami.MessageType.Error
        text: "Failed to delete Task"
        visible: false
    }
}
