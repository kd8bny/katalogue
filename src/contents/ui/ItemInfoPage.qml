// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 6.0
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

        print(recordData)
        nameField.text = recordData[3]
        makeField.text = recordData[4]
        modelField.text = recordData[5]
        yearField.text = recordData[6]

        typeBox.find(recordData[7])

        if (recordData[8]){
            itemArchived.checked = recordData[8]
            isArchived = true
        }

        if (recordData[9]){
            itemParentEnabled.checked = recordData[9]
            itemParentBox.find(recordData[9])
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
