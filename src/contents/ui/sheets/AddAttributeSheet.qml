// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import com.kd8bny.katalogue 1.0

Kirigami.OverlaySheet {
    id: addAttributeSheet

    // required property string uid

    parent: applicationWindow().overlay

    header: Kirigami.Heading {
        text: i18nc("@title:window", "Item Attribute")
    }

    Kirigami.FormLayout {
        Controls.TextField {
            id: keyField
            Kirigami.FormData.label: i18nc("@label:textbox", "Name:")
            //placeholderText: i18n("My Car")
            //onAccepted: nicknameField.forceActiveFocus()
        }
        Controls.TextField {
            id: valueField
            Kirigami.FormData.label: i18nc("@label:textbox", "Value:")
            //placeholderText: i18n("")
            //onAccepted: dateField.forceActiveFocus()
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
        Controls.Button {
            id: doneButton
            Layout.fillWidth: true
            text: i18nc("@action:button", "Add")
            enabled: (keyField.text.length & valueField.text.length) > 0
            onClicked: {
                Database.insertAttributeEntry(
                    groupField.text,
                    keyField.text,
                    valueField.text,
                )
                AttributeModel.updateModel()
                addAttributeSheet.close()
            }
        }
    }
}
