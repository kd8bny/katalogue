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

    title: i18n("Events")

    actions {
        main: Kirigami.Action {
            text: i18n("Add")
            icon.name: "list-add"
            tooltip: i18n("Add new event")
            onTriggered: {
                addEventSheet.item_id = item_id
                addEventSheet.open()
            }
        }
        contextualActions: [
            Kirigami.Action {
                text: i18n("Edit")
                icon.name: "entry-edit"
                tooltip: i18n("Edit item")
                onTriggered: pageStack.push("qrc:EditItemPage.qml", {"item_id": item_id})
            },
            Kirigami.Action {
                text: i18n("Attributes")
                icon.name: "item"
                tooltip: i18n("Swap to Attributes")
                onTriggered: pageStack.replace("qrc:Attributes.qml", {"itemName": itemName, "item_id": item_id})
            }
        ]
    }

    Component.onCompleted: EventModel.setItemID(item_id)

    Kirigami.CardsListView {
        id: layout
        model: EventModel
        delegate: eventDelegate

        headerPositioning: ListView.OverlayHeader
        header: Kirigami.ItemViewHeader {
            //backgroundImage.source: "../banner.jpg"
            title: itemName
        }

        Kirigami.PlaceholderMessage {
            anchors.centerIn: parent
            width: parent.width - (Kirigami.Units.largeSpacing * 4)

            visible: layout.count == 0
            text: i18n("Select add to add an event to this item")
        }
    }

    Component {
        id: eventDelegate

        Kirigami.AbstractCard {
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
                        Kirigami.Heading {
                            Layout.fillWidth: true
                            level: 2
                            text: date
                        }
                        Kirigami.Heading {
                            Layout.fillWidth: true
                            level: 2
                            text: event
                        }
                    }
                    Controls.Button {
                        Layout.alignment: Qt.AlignRight
                        // Layout.alignment: Qt.AlignBottom
                        Layout.columnSpan: 2
                        text: i18n("Edit")
                        // onClicked: to be done... soon!
                    }
                }
            }
        }
    }
}
