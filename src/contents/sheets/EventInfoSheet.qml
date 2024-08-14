// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import com.kd8bny.katalogue
import org.kde.kirigami as Kirigami

Kirigami.OverlaySheet {
    id: eventInfoSheet

    property alias date: dateField.text
    property alias event: eventField.text
    property alias cost: costField.text
    property alias increment: incrementField.text
    property alias category: categoryField.text
    property alias comment: commentField.text

    title: "Event"

    Kirigami.FormLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.preferredWidth: Kirigami.Units.gridUnit * 25
        Layout.preferredHeight: Kirigami.Units.gridUnit * 10

        Controls.Label {
            id: dateField

            Kirigami.FormData.label: i18nc("@label:textbox", "Date:")
        }

        Controls.Label {
            id: eventField

            Kirigami.FormData.label: i18nc("@label:textbox", "Event:")
        }

        Controls.Label {
            id: costField

            Kirigami.FormData.label: i18nc("@label:textbox", "Cost:")
        }

        Controls.Label {
            id: incrementField

            Kirigami.FormData.label: i18nc("@label:textbox", "Odometer:")
        }

        Controls.Label {
            id: categoryField

            Kirigami.FormData.label: i18nc("@label:textbox", "Category:")
        }

        Controls.Label {
            id: commentField

            Kirigami.FormData.label: i18nc("@label:textbox", "Comment:")
        }

    }

    footer: Controls.Button {
        Layout.fillWidth: true
        text: i18nc("@action:button", "Close")
        onClicked: {
            eventInfoSheet.close();
        }
    }

}
