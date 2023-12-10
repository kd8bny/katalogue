// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami
import com.kd8bny.katalogue 1.0


Kirigami.ScrollablePage {
    id: itemsPage

    property bool isComponentView: false

    Layout.fillWidth: true

    title: i18n("Katalogued Items")

    actions.contextualActions: [
        Kirigami.Action {
            text: i18n("Add Item")
            icon.name: "list-add"
            tooltip: i18n("Add new item")
            onTriggered: pageStack.push("qrc:AddEditItemPage.qml")
        }
    ]

    Component.onCompleted: {
        if(isComponentView){
            itemsLayout.model=ItemComponentModel
        }
    }

    Kirigami.CardsListView {
        id: itemsLayout
        model: ItemModel
        delegate: itemsDelegate

        Kirigami.PlaceholderMessage {
            anchors.centerIn: parent
            width: parent.width - (Kirigami.Units.largeSpacing * 4)

            visible: layout.count == 0
            text: i18n("Select add to set an item")
        }
    }

    Component {
        id: itemsDelegate

        Kirigami.Card {
            showClickFeedback: true
            onClicked:  {
                pageStack.push("qrc:Details.qml", {"itemModelIndex": index, "itemId": id, "itemName": name, "isComponentView": isComponentView})
            }

            banner {
                title: name
                titleIcon: "car"
            }

            contentItem: Item {
                // implicitWidth/Height define the natural width/height of an item if no width or height is specified.
                // The setting below defines a component's preferred size based on its content
                implicitWidth: delegateLayout.implicitWidth
                implicitHeight: delegateLayout.implicitHeight
                        ColumnLayout {

                        Controls.Label {
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                            text: i18n(year + " " + make + " " + model)
                        }
                        Kirigami.Separator {
                            Layout.fillWidth: true
                        }
                    }
                GridLayout {
                    id: delegateLayout
                    anchors {
                        left: parent.left
                        top: parent.top
                        right: parent.right
                    }
                    rowSpacing: Kirigami.Units.largeSpacing
                    columnSpacing: Kirigami.Units.largeSpacing
                    columns: root.wideScreen ? 4 : 2


                }
            }
        }
    }
}
