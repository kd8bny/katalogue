// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

import com.kd8bny.katalogue


Kirigami.OverlaySheet {
    id: attributeInfoSheet

    property string category
    property alias key: keyField.text
    property alias value: valueField.text

    title: category

    footer: Controls.Button {
        Layout.fillWidth: true
        text: i18nc("@action:button", "Close")
        onClicked: {
            attributeInfoSheet.close()
        }
    }

    Kirigami.FormLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true

        Layout.preferredWidth: Kirigami.Units.gridUnit * 25
        Layout.preferredHeight: Kirigami.Units.gridUnit * 5

        Controls.Label {
            id: keyField
            text: key

            Kirigami.FormData.label: i18nc("@label:textbox", "Name:")
        }
        Controls.Label {
            id: valueField
            Kirigami.FormData.label: i18nc("@label:textbox", "Value:")
            text: value
        }
    }
}
