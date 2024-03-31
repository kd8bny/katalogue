// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 6.0
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami
import com.kd8bny.katalogue 1.0


Kirigami.OverlaySheet {
    id: attributeInfoSheet

    property alias category: categoryField.text
    property alias key: keyField.text
    property alias value: valueField.text

    header: Kirigami.Heading {
        id: categoryField
    }

    footer: Controls.Button {
        Layout.fillWidth: true
        text: i18nc("@action:button", "Close")
        onClicked: {
            attributeInfoSheet.close()
        }
    }

    Kirigami.FormLayout {
        Controls.Label {
            id: keyField
            Kirigami.FormData.label: i18nc("@label:textbox", "Name:")
        }
        Controls.Label {
            id: valueField
            Kirigami.FormData.label: i18nc("@label:textbox", "Value:")
        }
    }
}
