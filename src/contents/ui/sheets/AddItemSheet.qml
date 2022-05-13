// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import com.kd8bny.katalogue 1.0

Kirigami.OverlaySheet {
    id: addItemSheet

    parent: applicationWindow().overlay

    header: Kirigami.Heading {
        text: i18nc("@title:window", "Add new item to katalogue")
    }

    Kirigami.FormLayout { // TODO required
        Controls.TextField {
            id: nameField
            Kirigami.FormData.label: i18nc("@label:textbox", "Name:")
            placeholderText: i18n("My Car")
            onAccepted: nicknameField.forceActiveFocus()
        }
        Controls.TextField {
            id: makeField
            Kirigami.FormData.label: i18nc("@label:textbox", "Make:")
            placeholderText: i18n("Ford")
            onAccepted: dateField.forceActiveFocus()
        }
        Controls.TextField {
            id: modelField
            Kirigami.FormData.label: i18nc("@label:textbox", "Model:")
            placeholderText: i18n("Mustang")
        }
        Controls.TextField {
            id: yearField
            Kirigami.FormData.label: i18nc("@label:textbox", "Year:")
            placeholderText: i18n("2000")
            //inputMask: "H"
            //onAccepted: dateField.forceActiveFocus()
        }
        Controls.TextField {
            id: categoryField
            Kirigami.FormData.label: i18nc("@label:textbox", "Make:")
            placeholderText: i18n("Optional")
        }
        Controls.TextField {
            id: groupField
            Kirigami.FormData.label: i18nc("@label:textbox", "Make:")
            placeholderText: i18n("Optional")
        }
        Controls.Button {
            id: doneButton
            Layout.fillWidth: true
            text: i18nc("@action:button", "Add")
            enabled: nameField.text.length > 0
            onClicked: {
                Database.insertItemEntry(
                    nameField.text,
                    makeField.text,
                    modelField.text,
                    yearField.text,
                    categoryField.text,
                    groupField.text
                )
                ItemModel.updateModel()
                addItemSheet.close()
            }
        }
    }
}
