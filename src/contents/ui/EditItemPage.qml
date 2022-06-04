// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import com.kd8bny.katalogue 1.0

Kirigami.ScrollablePage {
    id: addItemPage

    required property string item_id
    // required property string name
    // required property string make

    header: Kirigami.Heading {
        text: i18nc("@title:window", "Add new item to katalogue")
    }

    Kirigami.FormLayout {
        id: form

        Controls.TextField {
            id: nameField
            Kirigami.FormData.label: i18nc("@label:textbox", "Name:")
            placeholderText: i18n("required")
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
            //inputMask: "H"
        }
        Controls.ComboBox {
            editable: true
            Kirigami.FormData.label: i18nc("@label:textbox", "Category:")
            model: ListModel {
                id: categoryField
                ListElement { text: "Auto_ICE" }
                ListElement { text: "Utility" }
            }
            onAccepted: {
                if (find(editText) === -1)
                    model.append({text: editText})
            }
        }
        Controls.ComboBox {
            editable: true
            Kirigami.FormData.label: i18nc("@label:textbox", "Group:")
            model: ListModel {
                id: groupField
                ListElement { text: "Default" }
            }
            onAccepted: {
                if (find(editText) === -1)
                    model.append({text: editText})
            }
        }
        Controls.CheckBox {
            id: archivedBox
            Kirigami.FormData.label: i18nc("@label:textbox", "Archive:")
        }
        Controls.Button {
            id: doneButton
            Layout.fillWidth: true
            text: i18nc("@action:button", "Update")
            enabled: nameField.text.length > 0
            onClicked: {
                Database.insertItemEntry(
                    nameField.text,
                    makeField.text,
                    modelField.text,
                    yearField.text,
                    categoryField.text,
                    groupField.text,
                    archivedBox.checked
                )
                ItemModel.updateModel()
                pageStack.pop()
            }
        }
        Controls.Button {
            id: deleteButton
            Layout.fillWidth: true
            text: i18nc("@action:button", "Delete")
            onClicked: {
                Database.deleteItemEntry(item_id)
                ItemModel.updateModel()
                pageStack.clear()
                pageStack.push("qrc:Items.qml")
            }
        }


    }
}
