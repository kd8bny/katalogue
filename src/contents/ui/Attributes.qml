// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import com.kd8bny.katalogue


Kirigami.ScrollablePage {
    id: page

    required property string itemId
    required property string itemName

    title: i18n(itemName + " Attributes")

    actions: [
        Kirigami.Action {
            text: i18n("Add")
            icon.name: "list-add"
            tooltip: i18n("Add new attribute")
            onTriggered: {
                pageStack.push("qrc:AddEditAttributePage.qml", {"itemId": itemId})
            }
        }
    ]

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
            Kirigami.IconTitleSubtitle {
                Layout.fillWidth: true
                title: section
                // icon: icon.fromControlsIcon(subtitleDelegate.icon)
            }
        }
    }

    Component {
        id: attributeDelegate

        Kirigami.SubtitleDelegate {
            id: subtitleDelegate
            Layout.fillWidth: true

            text: key
            subtitle: value

            contentItem: RowLayout {
                Rectangle {
                    radius: height
                    Layout.preferredWidth: Kirigami.Units.largeSpacing
                    Layout.preferredHeight: Kirigami.Units.largeSpacing
                    color: Kirigami.Theme.neutralTextColor
                }
                Kirigami.IconTitleSubtitle {
                    Layout.fillWidth: true
                    title: subtitleDelegate.text
                    subtitle: subtitleDelegate.subtitle
                    icon: icon.fromControlsIcon(subtitleDelegate.icon)
                }
                Controls.Button {
                    Layout.rightMargin: Kirigami.Units.gridUnit
                    icon.name: "kdocumentinfo"
                    onClicked: {
                        openInfoSheet(index)
                    }
                }
                Controls.Button {
                    Layout.rightMargin: Kirigami.Units.gridUnit
                    icon.name: "edit-entry"
                    onClicked: {
                        pageStack.push("qrc:AddEditAttributePage.qml", {
                            "itemId": itemId,
                            "isEdit": true,
                            "attributeIndex": index
                        })
                    }
                }
            }
        }
    }
}
