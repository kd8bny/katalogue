// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import com.kd8bny.katalogue
import com.kd8bny.katalogue.entries
import org.kde.kirigami as Kirigami

Kirigami.ScrollablePage {
    id: itemInfoPage

    required property EntryItem entryItem

    title: i18n(entryItem.name + " Details")
    Component.onCompleted: {
        nameField.text = entryItem.name;
        makeField.text = entryItem.make;
        modelField.text = entryItem.model;
        yearField.text = entryItem.year;
        typeBox.text = entryItem.type;
        itemParentBox.text = entryItem.itemId; //TODO name not number
    }
    actions: [
        Kirigami.Action {
            //TODO update data on exit

            text: i18n("Edit")
            icon.name: "entry-edit"
            tooltip: i18n("Edit item")
            onTriggered: {
                pageStack.push("qrc:AddEditItemPage.qml", {
                    "entryItem": entryItem
                });
            }
        }
    ]

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
            // TODO visable

            id: itemParentBox

            Kirigami.FormData.label: i18nc("@label:textbox", "Parent Item:")
        }

    }

}
