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
    // DatePopup {
    //     //root.incidenceWrapper.setIncidenceStartDate(day, month, year)
    //     // Layout.fillWidth: true
    //     // display: root.incidenceWrapper.incidenceStartDateDisplay
    //     // dateTime: root.incidenceWrapper.incidenceStart
    //     // onNewDateChosen: console.log("hi" + day)
    //     id: dateDialog
    // }

    id: addEditEventPage

    required property EntryItem entryItem
    property EntryEvent entryEvent
    property bool isEdit: false

    function insertUpdate() {
        entryEvent.date = `${dateField.text}T${Qt.formatDateTime(new Date(), Qt.ISODate).split("T")[1]}`;
        entryEvent.event = eventField.text;
        entryEvent.servicer = servicerBox.text;
        entryEvent.cost = costField.text;
        entryEvent.increment = incrementField.text;
        if (servicerBox.find(servicerBox.editText) === -1)
            entryEvent.category = servicerBox.editText;
        else
            entryEvent.category = servicerBox.currentText;
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
        dateField.text = Qt.formatDateTime(new Date(), Qt.ISODate).split("T")[0];
        if (entryEvent) {
            isEdit = true;
            dateField.text = entryEvent.date;
            eventField.text = entryEvent.event;
            servicerBox.text = entryEvent.servicer;
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

    Controls.Action {
        id: addUpdateAction

        shortcut: "Return"
        onTriggered: {
            if (insertUpdate()) {
                EventModel.refreshModel();
                EventCategoryModel.refreshModel();
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
            if (EventDatabase.deleteEntryById(EventModel.getId(entryEvent.id))) {
                EventModel.refreshModel();
                EventCategoryModel.refreshModel();
                pageStack.pop();
            } else {
                msgDeleteError.visible = true;
            }
        }
        onRejected: {
            pageStack.pop();
        }
    }

    ColumnLayout {
        Kirigami.InlineMessage {
            id: msgInsertUpdateError

            Layout.fillWidth: true
            type: Kirigami.MessageType.Error
            text: (isEdit) ? i18n("Failed to update Event") : i18n("Failed to insert Event")
            visible: false
        }

        Kirigami.InlineMessage {
            id: msgDateError

            Layout.fillWidth: true
            type: Kirigami.MessageType.Error
            text: "Invalid Date"
            visible: false
        }

        Kirigami.InlineMessage {
            id: msgDeleteError

            Layout.fillWidth: true
            type: Kirigami.MessageType.Error
            text: "Failed to delete Event"
            visible: false
        }

        Kirigami.FormLayout {
            // Controls.Button {
            //     id: dateButton
            //     // Layout.fillWidth: true
            //     text: i18nc("@action:button", "date")
            //     onClicked: {
            //         dateDialog.open();
            //     }
            // } //TODO date popup

            id: form

            Layout.fillWidth: true
            Kirigami.FormData.label: i18n("Date:")

            Controls.TextField {
                id: dateField

                Kirigami.FormData.label: i18nc("@label:textbox", "Date:")
                inputMask: "D999-99-99"
                onEditingFinished: {
                    let inputDate = new Date(dateField.text);
                    if (isNaN(inputDate.getDate()))
                        msgDateError.visible = true;
                    else
                        msgDateError.visible = false;
                }
            }

            Controls.TextField {
                id: eventField

                Kirigami.FormData.label: i18nc("@label:textbox", "Event:")
            }

            Kirigami.Separator {
                Kirigami.FormData.isSection: true
            }

            Controls.ComboBox {
                id: servicerBox

                Layout.fillWidth: true
                Kirigami.FormData.label: i18nc("@label:textbox", "Servicer:")
                editable: true
                model: UniqueValueModelFactory.createEventServicerModel(entryItem.type)
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
                model: UniqueValueModelFactory.createEventCategoryModel(entryItem.type)
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

    }

    footer: Controls.DialogButtonBox {
        standardButtons: Controls.DialogButtonBox.Cancel
        onRejected: pageStack.pop()
        onAccepted: addUpdateAction.trigger()

        Controls.Button {
            Controls.DialogButtonBox.buttonRole: Controls.DialogButtonBox.AcceptRole
            enabled: (eventField.text.length > 0) && !msgDateError.visible
            icon.name: isEdit ? "document-save" : "list-add"
            text: isEdit ? i18n("Save") : i18n("Add")
        }

    }

}
