// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import com.kd8bny.katalogue 1.0


Kirigami.ScrollablePage {
    id: page

    required property string item_id
    required property string itemName

    Layout.fillWidth: true

    title: i18n(itemName)

    // actions {
    //     main: Kirigami.Action {
    //             text: i18n("Add")
    //             icon.name: "list-add"
    //             tooltip: i18n("Add new attribute")
    //             onTriggered: addAttributeSheet.open()
    //         }
    //     left: Kirigami.Action {
    //         text: i18n("Edit")
    //         icon.name: "entry-edit"
    //         tooltip: i18n("Edit item")
    //         onTriggered: pageStack.push("qrc:EditItemPage.qml", {"item_id": item_id})
    //     }
    //     right: Kirigami.Action {
    //         text: i18n("swap")
    //         icon.name: "entry-edit"
    //         tooltip: i18n("Edit item")
    //         onTriggered: loader.source = "Events.qml"
    //     }
    // }

    Loader{
        id: loader
        anchors.top: page.top
        anchors.left: page.left
        anchors.right: page.right
        anchors.bottom: page.bottom
        anchors.topMargin: 0
        source: "Attributes.qml"
    }

}
