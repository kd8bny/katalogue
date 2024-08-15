// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import com.kd8bny.katalogue
import com.kd8bny.katalogue.entries
import org.kde.kirigami as Kirigami

Kirigami.ScrollablePage {
    id: addEditEventPage

    required property EntryItem entryItem
    property EntryEvent entryEvent
    property bool isEdit: false

    function insertUpdate() {
        entryEvent.date = dateField.text;
        entryEvent.event = eventField.text;
        entryEvent.cost = costField.text;
        entryEvent.increment = incrementField.text;
        if (categoryBox.find(categoryBox.editText) === -1)
            entryEvent.category = categoryBox.editText;
        else
            entryEvent.category = categoryBox.currentText;
        entryEvent.comment = commentField.text;
        entryEvent.itemId = entryItem.id;
        if (isEdit)
            return EventDatabase.updateEntry(entryEvent);
        else
            return EventDatabase.insertEntry(entryEvent);
    }

    title: (isEdit) ? i18n("Edit Event") : i18n("Add Event")
    Component.onCompleted: {
        var locale = Qt.locale();
        var currentDate = new Date();
        var dateString = currentDate.toLocaleDateString(locale, Locale.ShortFormat);
        dateField.text = Qt.formatDateTime(currentDate, Qt.ISODate).split("T")[0]; //TODO adjust in event
        if (entryEvent) {
            isEdit = true;
            dateField.text = entryEvent.date;
            eventField.text = entryEvent.event;
            costField.text = entryEvent.cost; // TODO `${recordData[5]}`;
            incrementField.text = entryEvent.increment; //`${recordData[6]}`;
            categoryBox.currentIndex = categoryBox.find(entryEvent.category);
            commentField.text = entryEvent.comment;
        } else {
            entryEvent = EventDatabase.getNewEntry();
        }
    }
    actions: [
        Kirigami.Action {
            visible: isEdit
            text: i18n("Delete")
            icon.name: "delete"
            tooltip: i18n("Remove Event")
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
            if (EventModel.deleteEntryById(EventModel.getId(entryEvent.id))) {
                EventModel.refresh();
                EventCategoryModel.refresh();
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

        Controls.TextField {
            id: eventField

            Kirigami.FormData.label: i18nc("@label:textbox", "Event:")
        }

        Controls.TextField {
            id: costField

            Kirigami.FormData.label: i18nc("@label:textbox", "Cost:")
        }

        Controls.TextField {
            id: incrementField

            Kirigami.FormData.label: i18nc("@label:textbox", "Increment:") //TODO determine name on schema
        }

        Controls.ComboBox {
            id: categoryBox

            Kirigami.FormData.label: i18nc("@label:textbox", "Category")
            editable: true
            model: EventCategoryModel
        }

        Controls.TextArea {
            id: commentField

            Layout.fillWidth: true
            Kirigami.FormData.label: i18nc("@label:textbox", "Comment")
        }

        Controls.Button {
            id: doneButton

            Layout.fillWidth: true
            text: (isEdit != -1) ? i18nc("@action:button", "Update") : i18nc("@action:button", "Add")
            enabled: eventField.text.length > 0
            onClicked: {
                if (insertUpdate()) {
                    EventModel.refresh();
                    EventCategoryModel.refresh();
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
        text: (isEdit) ? i18n("Failed to update Event") : i18n("Failed to insert Event")
        visible: false
    }

    Kirigami.InlineMessage {
        id: msgDeleteError

        type: Kirigami.MessageType.Error
        text: "Failed to delete Event"
        visible: false
    }

}
