// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami
import com.kd8bny.katalogue 1.0

Kirigami.ScrollablePage {
    id: addEditEventPage

    required property int itemId
    property int eventIndex: -1

    title: (eventIndex != -1) ? i18n("Edit Event") : i18n("Add Event")

    actions {
        main: Kirigami.Action {
            enabled: eventIndex != -1
            text: i18n("Delete")
            icon.name: "delete"
            tooltip: i18n("Remove Attribute")
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
            EventModel.deleteRecord(EventModel.getId(eventIndex))
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
        dateField.text = dateString

        if (eventIndex != -1) {
            var recordData = EventModel.getRecordAsList(eventIndex)

            dateField.text = recordData[1]
            eventField.text = recordData[2]
            costField.text = recordData[3]
            odometerField.text = recordData[4]
            categoryBox.find(recordData[5])
            commentField.text = recordData[6]
        }
    }

    ColumnLayout{
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
                id: odometerField
                Kirigami.FormData.label: i18nc("@label:textbox", "Odometer:")
            }
            Controls.ComboBox {
                id: categoryBox
                Kirigami.FormData.label: i18nc("@label:textbox", "Category")
                editable: true
                model: EventCategoryModel
            }

            Controls.ScrollView {
                id: view
                Kirigami.FormData.label: i18nc("@label:textbox", "Category")
                // anchors.fill: parent
                // Layout.fillWidth: true
                width: Kirigami.Units.gridUnit * 12
                height: Kirigami.Units.gridUnit * 6
                // Layout.minimumWidth: Kirigami.Units.gridUnit * 12
                // Layout.minimumHeight: Kirigami.Units.gridUnit * 12
                // ScrollBar.vertical.interactive: true
                Controls.TextArea {
                    Layout.fillWidth: true
                    placeholderText: i18nc("Comment")
                    // Layout.height: 6
                    id: commentField

                    // Layout.height: Kirigami.Units.gridUnit * 12
                    wrapMode: Text.WordWrap
                }
            }
            Controls.Button {
                id: doneButton
                Layout.fillWidth: true
                text: (eventIndex != -1) ? i18nc("@action:button", "Update") : i18nc("@action:button", "Add")
                // enabled: (keyField.text.length & valueField.text.length) > 0
                onClicked: {
                    var category = ""
                    if (categoryBox.find(categoryBox.editText) === -1){
                        category = categoryBox.editText
                    }else{
                        category = categoryBox.currentText
                    }

                    EventModel.setRecord(
                        eventIndex,
                        dateField.text,
                        eventField.text,
                        parseFloat(costField.text),
                        parseFloat(odometerField.text),
                        category,
                        commentField.text,
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
