// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import com.kd8bny.katalogue 1.0


Kirigami.ScrollablePage {
    id: itemsPage

    Layout.fillWidth: true

    title: i18n("Katalogued Items")

    actions {
        main: Kirigami.Action {
            text: i18n("Add Item")
            icon.name: "list-add"
            tooltip: i18n("Add new item")
            onTriggered: pageStack.layers.push("qrc:AddItemPage.qml")
        }
    }

    Kirigami.CardsListView {
        id: layout
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

            banner {
                title: name
                titleIcon: "car"
            }

            contentItem: Item {
                // implicitWidth/Height define the natural width/height of an item if no width or height is specified.
                // The setting below defines a component's preferred size based on its content
                implicitWidth: delegateLayout.implicitWidth
                implicitHeight: delegateLayout.implicitHeight

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

                    ColumnLayout {

                        Controls.Label {
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                            text: year + " " + model + " " + make
                        }
                        Kirigami.Separator {
                            Layout.fillWidth: true
                        }
                    }
                }
            }
            actions: [
                  Kirigami.Action {
                    text: "Details"
                    icon.name: "item"

                    onTriggered: pageStack.push("qrc:Attributes.qml", {"itemName": name, "item_id": id})
                },
                Kirigami.Action {
                    text: "Maintence"
                    icon.name: "item"

                    onTriggered: pageStack.push("qrc:Events.qml", {"itemName": name, "item_id": id})
                }
            ]
        }
    }
}
