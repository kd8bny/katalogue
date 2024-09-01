// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import com.kd8bny.katalogue
import com.kd8bny.katalogue.entries
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.dateandtime

Kirigami.ScrollablePage {
    id: addEditEventPage

    required property EntryItem entryItem
    property EntryEvent entryEvent
    property bool isEdit: false

    function insertUpdate() {
        entryEvent.date = dateField.text;
        entryEvent.event = eventField.text;
        entryEvent.cost = costField.text;
        entryEvent.increment = incrementField.value;
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
            costField.text = entryEvent.cost;
            incrementField.text = entryEvent.increment;
            categoryBox.currentIndex = categoryBox.find(entryEvent.category);
            commentField.text = entryEvent.comment;
        } else {
            entryEvent = EventDatabase.getEntryById();
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

    DatePopup {
        // Layout.fillWidth: true
        // display: root.incidenceWrapper.incidenceStartDateDisplay
        // dateTime: root.incidenceWrapper.incidenceStart
        // onNewDateChosen: root.incidenceWrapper.setIncidenceStartDate(day, month, year)

        id: dateDialog
    }

    Controls.Action {
        id: addUpdateAction

        enabled: eventField.text.length > 0
        shortcut: "Return"
        onTriggered: {
            if (insertUpdate()) {
                EventModel.refresh();
                EventCategoryModel.refresh();
                pageStack.pop();
            } else {
                msgInsertUpdateError.visible = true;
            }
        }
    }

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

        RowLayout {
            Layout.fillWidth: true
            Kirigami.FormData.label: i18n("Date:")

            Controls.TextField {
                id: dateField

                Kirigami.FormData.label: i18nc("@label:textbox", "Date:")
            }

            Controls.Button {
                id: dateButton

                // Layout.fillWidth: true
                text: i18nc("@action:button", "date")
                onClicked: {
                    dateDialog.open();
                }
            }

        }

        Controls.TextField {
            id: eventField

            Kirigami.FormData.label: i18nc("@label:textbox", "Event:")
        }

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
        }

        Controls.TextField {
            id: costField

            Layout.fillWidth: true
            Kirigami.FormData.label: i18nc("@label:textbox", "Cost:")
            text: Number("0.00").toFixed(2)
            onEditingFinished: {
                costField.text = parseFloat(costField.text).toFixed(2);
            }
        }

        Controls.TextField {
            id: incrementField

            Layout.fillWidth: true
            Kirigami.FormData.label: i18nc("@label:textbox", "Increment:")
            text: Number("0.0").toFixed(1)
            onEditingFinished: {
                incrementField.text = parseFloat(incrementField.text).toFixed(1);
            }
        }

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
        }

        Controls.ComboBox {
            id: categoryBox

            Layout.fillWidth: true
            Kirigami.FormData.label: i18nc("@label:textbox", "Category:")
            editable: true
            model: EventCategoryModel
        }

        RowLayout {
            Layout.fillWidth: true
            Kirigami.FormData.label: i18nc("@label:textbox", "Comment:")
            Kirigami.FormData.labelAlignment: Qt.AlignTop

            Flickable {
                // width: parent.width
                // height: Kirigami.Units.gridUnit * 8
                // clip: false

                id: commentFieldParent

                Layout.fillWidth: true
                height: Math.min(contentHeight, Kirigami.Units.gridUnit * 8)
                contentWidth: width
                contentHeight: commentField.implicitHeight

                Controls.TextArea.flickable: Controls.TextArea {
                    id: commentField

                    wrapMode: Text.WordWrap //TODO style isnt correct in flickable
                }

                Controls.ScrollBar.vertical: Controls.ScrollBar {
                }

            }

        }

    }

    footer: Controls.DialogButtonBox {
        standardButtons: Controls.DialogButtonBox.Cancel
        onRejected: pageStack.pop()
        onAccepted: addUpdateAction.trigger()

        Controls.Button {
            icon.name: isEdit ? "document-save" : "list-add"
            text: isEdit ? i18n("Save") : i18n("Add")
            Controls.DialogButtonBox.buttonRole: Controls.DialogButtonBox.AcceptRole
            enabled: eventField.length > 0
        }

    }

}
