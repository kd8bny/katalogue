// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami

Kirigami.OverlaySheet {
	id: addItemSheet

    parent: applicationWindow().overlay

	header: Kirigami.Heading {
		text: i18nc("@title:window", "Add new item to katalogue")
	}
	Kirigami.FormLayout {
		Controls.TextField {
			id: nameField
			Kirigami.FormData.label: i18nc("@label:textbox", "Name:")
			placeholderText: i18n("Item name (required)")
			onAccepted: nicknameField.forceActiveFocus()
		}
		Controls.TextField {
			id: makeField
			Kirigami.FormData.label: i18nc("@label:textbox", "Make:")
			placeholderText: i18n("Optional")
			onAccepted: dateField.forceActiveFocus()
		}
        Controls.TextField {
			id: modelField
			Kirigami.FormData.label: i18nc("@label:textbox", "Model:")
			placeholderText: i18n("Optional")
			onAccepted: dateField.forceActiveFocus()
		}
		Controls.TextField {
			id: yearField
			Kirigami.FormData.label: i18nc("@label:textbox", "Year:")
			placeholderText: i18n("Year YYYY")
			inputMask: "0000"
		}
        Controls.TextField {
			id: nicknameField
			Kirigami.FormData.label: i18nc("@label:textbox", "Make:")
			placeholderText: i18n("Optional")
			onAccepted: dateField.forceActiveFocus()
		}
		//TODO category
		Controls.Button {
			id: doneButton
			Layout.fillWidth: true
			text: i18nc("@action:button", "Add")
			enabled: nameField.text.length > 0
			onClicked: {
                // Item item;
                // item.name = nameField.text
                // item.name = nameField.text
				// if(database.insertItemEntry(item)){
				// 	addItemSheet.close();
				// }
				// kountdownModel.append({
				// 	name: nameField.text,
				// 	description: descriptionField.text,
				// 	//The parse() method parses a string and returns the number of milliseconds since January 1, 1970, 00:00:00 UTC.
				// 	//date: Date.parse(dateField.text)
				// });
				// nameField.text = ""
				// descriptionField.text = ""
				// dateField.text = ""
			}
		}
	}
}
