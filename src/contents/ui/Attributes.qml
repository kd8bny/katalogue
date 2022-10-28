// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import com.kd8bny.katalogue 1.0


Kirigami.ScrollablePage {
    id: page

    required property string itemId
    required property string itemName

    title: i18n("Attributes")

    actions {
        main: Kirigami.Action {
            text: i18n("Add")
            icon.name: "list-add"
            tooltip: i18n("Add new attribute")
            onTriggered: {
                pageStack.push("qrc:AddEditAttributePage.qml", {"itemId": itemId})
            }
        }
        contextualActions: [
            Kirigami.Action {
                text: i18n("Events")
                icon.name: "item"
                tooltip: i18n("Swap to Events")
                onTriggered: pageStack.replace("qrc:Events.qml", {"itemName": itemName, "itemId": itemId, "isEdit": true})
            }
        ]
    }

    Kirigami.CardsListView {
        id: layout
        model: AttributeModel
        delegate: attributeDelegate

        headerPositioning: ListView.OverlayHeader
        header: Kirigami.ItemViewHeader {
            //backgroundImage.source: "../banner.jpg"
            title: itemName
        }

        Kirigami.PlaceholderMessage {
            anchors.centerIn: layout
            width: layout.width - (Kirigami.Units.largeSpacing * 4)

            visible: layout.count == 0
            text: i18n("Select add to add an attribute to this item")
        }
    }

    Component.onCompleted: AttributeModel.setItemId(itemId)

    Component {
        id: attributeDelegate

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

                    ColumnLayout{
                        Controls.Label {
                            wrapMode: Text.WordWrap
                            text: "Category: " + category
                        }
                        RowLayout{
                            Kirigami.Heading {
                                wrapMode: Text.WordWrap
                                level: 2
                                text: key + ": "
                            }
                            Kirigami.Heading {
                                Layout.fillWidth: true
                                wrapMode: Text.WordWrap
                                text: value
                            }
                            Controls.Button {
                                Layout.alignment: Qt.AlignRight
                                // Layout.alignment: Qt.AlignBottom
                                Layout.columnSpan: 2
                                text: i18n("Edit")
                                onClicked: {
                                    pageStack.push("qrc:AddEditAttributePage.qml", {
                                        "itemId": itemId,
                                        "isEdit": true,
                                        "attributeId": index
                                    })
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
