// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami
import com.kd8bny.katalogue 1.0

Kirigami.ScrollablePage {  //TODO whole page layout
    id: addEditItemPage

    required property int itemModelIndex  // Item Model index position

    title: "Details"

    actions.contextualActions: [
        Kirigami.Action {
            text: i18n("Edit")
            icon.name: "entry-edit"
            tooltip: i18n("Edit item")

            onTriggered: {
                pageStack.push("qrc:AddEditItemPage.qml", {"itemId": itemId, "itemModelIndex": itemModelIndex})
            }
        }
    ]

    Component.onCompleted: {
        var recordData = ItemModel.getRecordAsList(itemModelIndex)

        nameField.text = recordData[1]
        makeField.text = recordData[2]
        modelField.text = recordData[3]
        yearField.text = recordData[4]

        typeBox.find(recordData[5])

        if (recordData[6]){
            itemArchived.checked = recordData[6]
            isArchived = true
        }

        if (recordData[7]){
            itemParentEnabled.checked = recordData[6]
            itemParentBox.find(recordData[7])
        }
    }

    Kirigami.FormLayout {
        id: form

        Controls.TextField {
            id: nameField
            Kirigami.FormData.label: i18nc("@label:textbox", "Name:")
            placeholderText: i18n("Required")
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
            placeholderText: i18n("YYYY")
            inputMask: "9999"
            text: "2000"
        }
        Controls.ComboBox {
            id: typeBox
            Kirigami.FormData.label: i18nc("@label:textbox", "Type:")
            editable: true
            model: ItemTypeModel
        }
        Controls.Switch {
            id: itemParentEnabled
            Kirigami.FormData.label: i18nc("@label:textbox", "Child Item:")
        }
        Controls.ComboBox {
            id: itemParentBox
            editable: false
            enabled: itemParentEnabled.checked
            model: ItemParentModel
            Kirigami.FormData.label: i18nc("@label:textbox", "Parent Item")
        }
    }
}
