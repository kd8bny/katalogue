// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami
import com.kd8bny.katalogue 1.0


Kirigami.ScrollablePage {
    id: page

    required property int itemModelIndex  // Item Model index position
    required property int itemId          // Item Id from Database
    required property string itemName     // Item name field
    property bool isComponentView         // Component view or Item level view

    title: i18n(itemName)

    actions.contextualActions: [
        Kirigami.Action {
            text: i18n("Home")
            icon.name: "go-home-symbolic"
            tooltip: i18n("Go back to items page")
            onTriggered: {
                pageStack.clear()
                pageStack.push("qrc:Items.qml")
            }
        }
    ]

    Component.onCompleted: {
        if(isComponentView){
            menuModel.remove(2)  // Remove Components if Components
        }
        pageStack.push("qrc:Attributes.qml", {"itemName": itemName, "itemId": itemId})
    }

    ListModel {
        id: menuModel
        ListElement { name: "Attributes" }
        ListElement { name: "Events" }
        ListElement { name: "Components" }
        ListElement { name: "Notes" }
        ListElement { name: "Details" }
    }

    ListView {
        id: menuList

        model: menuModel
        delegate: itemDelegate
    }

    Component {
        id: itemDelegate

        Kirigami.BasicListItem {
            label: i18n(name)

            onClicked: {
                if(name == "Attributes"){
                    pageStack.push("qrc:Attributes.qml", {"itemName": itemName, "itemId": itemId})
                }else if(name == "Events"){
                    pageStack.push("qrc:Events.qml", {"itemName": itemName, "itemId": itemId})
                }else if(name == "Components"){
                    pageStack.push("qrc:Items.qml", {"isComponentView": true})
                    ItemComponentModel.filterComponent()
                }else if(name == "Notes"){
                    pageStack.push("qrc:Notes.qml", {"itemName": itemName, "itemId": itemId})
                }else if(name == "Details"){
                    pageStack.push('qrc:ItemInfoPage.qml', {"itemModelIndex": itemModelIndex})
                }
            }
        }
    }

}
