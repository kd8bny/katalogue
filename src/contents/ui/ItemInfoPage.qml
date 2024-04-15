// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

import com.kd8bny.katalogue

Kirigami.ScrollablePage {
    id: itemInfoPage

    required property int itemModelIndex  // Item Model index position

    title: "Details"

    actions: [
        Kirigami.Action {
            text: i18n("Edit")
            icon.name: "entry-edit"
            tooltip: i18n("Edit item")

            onTriggered: {
                pageStack.push("qrc:AddEditItemPage.qml", {"isEdit": true, "itemModelIndex": itemModelIndex})
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

        typeBox.text = recordData[7]
        itemParentBox.text = recordData[9]  //TODO name not number

    }

    Kirigami.FormLayout {
        id: form

        Controls.Label {
            id: nameField
            Kirigami.FormData.label: i18nc("@label:textbox", "Name:")
        }
        Controls.Label {
            id: makeField
            Kirigami.FormData.label: i18nc("@label:textbox", "Make:")
        }
        Controls.Label {
            id: modelField
            Kirigami.FormData.label: i18nc("@label:textbox", "Model:")
        }
        Controls.Label {
            id: yearField
            Kirigami.FormData.label: i18nc("@label:textbox", "Year:")
        }
        Controls.Label {
            id: typeBox
            Kirigami.FormData.label: i18nc("@label:textbox", "Type:")
        }
        Controls.Label {
            id: itemParentBox
            Kirigami.FormData.label: i18nc("@label:textbox", "Parent Item:")
            // TODO visable
        }
    }
}
