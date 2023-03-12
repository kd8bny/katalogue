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

    title: i18n(itemName + " Attributes")

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

    Component.onCompleted: AttributeModel.setItemId(itemId)

    ListView {
        id: layout
        model: AttributeModel
        delegate: attributeDelegate
        section.property: "category"
        section.delegate: sectionDelegate
        focus: true

        // headerPositioning: ListView.OverlayHeader
        // header: itemName

        Kirigami.PlaceholderMessage {
            anchors.centerIn: layout
            width: layout.width - (Kirigami.Units.largeSpacing * 4)

            visible: layout.count == 0
            text: i18n("Select add to add an attribute to this item")
        }
    }

    Component {
        id: sectionDelegate
        Kirigami.ListSectionHeader {
            label: section
        }
    }

    Component {
        id: attributeDelegate

        Kirigami.SwipeListItem {
            separatorVisible: true

            actions: [
                Kirigami.Action {
                    icon.name: "kdocumentinfo"
                    onTriggered: {
                        infoSheet.open({"attributeIndex": index})
                    }
                },
                Kirigami.Action {
                    icon.name: "edit-entry"
                    onTriggered: {
                        pageStack.push("qrc:AddEditAttributePage.qml", {
                            "itemId": itemId,
                            "isEdit": true,
                            "attributeIndex": index
                        })
                    }
                }
            ]

            RowLayout {
                id: delegateLayout
                Controls.Label {
                    wrapMode: Text.WordWrap
                    text: key + ": "
                }
                Controls.Label {
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                    text: value
                }
            }
        }
    }

    Kirigami.OverlaySheet {
        id: infoSheet

        // required property string attributeIndex

        // parent: applicationWindow().overlay

        header: Kirigami.Heading {
            text: "Attribute"
        }

        footer: Controls.Button {
            Layout.fillWidth: true
            text: i18nc("@action:button", "Close")
            onClicked: {
                infoSheet.close()
            }
        }

        Component.onCompleted: {
            var recordData = AttributeModel.getRecord(0)
            keyField.text = recordData[1]
            valueField.text = recordData[2]
            categoryField.text = recordData[3]
        }

        ColumnLayout {
            Controls.Label {
                id: categoryField
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
            }
            RowLayout {
                Layout.fillWidth: true
                Controls.Label {
                    id: keyField
                    wrapMode: Text.WordWrap
                }
                    Controls.Label {
                    id: valueField
                    wrapMode: Text.WordWrap
                }
            }
        }
    }
}
