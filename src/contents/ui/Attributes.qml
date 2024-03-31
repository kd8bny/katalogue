// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 6.0
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami
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
    }

    function openInfoSheet(index = -1) {
        var recordData = AttributeModel.getRecordAsList(index)

        attributeInfoSheet.key = recordData[3]
        attributeInfoSheet.value = recordData[4]
        attributeInfoSheet.category = recordData[5]

        attributeInfoSheet.open()
    }

    Component.onCompleted: AttributeModel.setItemId(itemId)

    ListView {
        id: layout
        model: AttributeModel
        delegate: attributeDelegate
        section.property: "category"
        section.delegate: sectionDelegate
        focus: true

        headerPositioning: ListView.OverlayHeader
        header: itemName

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
                        openInfoSheet(index)
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
}
