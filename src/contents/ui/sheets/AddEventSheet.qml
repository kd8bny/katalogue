// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami


Kirigami.OverlaySheet {
	id: addEventSheet
	header: Kirigami.Heading {
		text: i18nc("@title:window", "Add kountdown")
	}
	Kirigami.FormLayout {
		Controls.TextField {
			id: nameField
			Kirigami.FormData.label: i18nc("@label:textbox", "Name:")
			placeholderText: i18n("Event name (required)")
			onAccepted: descriptionField.forceActiveFocus()
		}
		Controls.TextField {
			id: descriptionField
			Kirigami.FormData.label: i18nc("@label:textbox", "Description:")
			placeholderText: i18n("Optional")
			onAccepted: dateField.forceActiveFocus()
		}
		Controls.TextField {
			id: dateField
			Kirigami.FormData.label: i18nc("@label:textbox", "Date:")
			placeholderText: i18n("YYYY-MM-DD")
			inputMask: "0000-00-00"
		}
		Controls.Button {
			id: doneButton
			Layout.fillWidth: true
			text: i18nc("@action:button", "Add")
			enabled: nameField.text.length > 0
			onClicked: {
				kountdownModel.append({
					name: nameField.text, 
					description: descriptionField.text, 
					//The parse() method parses a string and returns the number of milliseconds since January 1, 1970, 00:00:00 UTC.
					date: Date.parse(dateField.text)
				});
				nameField.text = ""
				descriptionField.text = ""
				dateField.text = ""
				addSheet.close();
			}
		}
	}
}
