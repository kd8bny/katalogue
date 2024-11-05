// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import com.kd8bny.katalogue
import com.kd8bny.katalogue.entries
import org.kde.kirigami as Kirigami

Kirigami.ScrollablePage {
    id: attributesPage

    required property EntryItem entryItem

    function getEntryByIndex(index) {
        return AttributeDatabase.getEntryById(AttributeModel.getId(index));
    }

    function openInfoSheet(index) {
        var attribute = getEntryByIndex(index);
        attributeInfoSheet.key = attribute.key;
        attributeInfoSheet.value = attribute.value;
        attributeInfoSheet.category = attribute.category;
        attributeInfoSheet.open();
    }

    Layout.fillWidth: true
    title: i18n(entryItem.name + " Attributes")
    Component.onCompleted: AttributeModel.setItemIdQuery(entryItem.id)
    actions: [
        Kirigami.Action {
            text: i18n("Add")
            icon.name: "list-add"
            tooltip: i18n("Add new attribute")
            onTriggered: {
                pageStack.push("qrc:AddEditAttributePage.qml", {
                    "entryItem": entryItem
                });
            }
        }
    ]

    ListView {
        id: layout

        model: AttributeModel
        section.property: "category"
        focus: true
        headerPositioning: ListView.OverlayHeader

        Kirigami.PlaceholderMessage {
            anchors.centerIn: layout
            width: layout.width - (Kirigami.Units.largeSpacing * 4)
            visible: layout.count == 0
            text: i18n("Select add to add an attribute to this item")
        }

        delegate: Kirigami.SubtitleDelegate {
            id: subtitleDelegate

            width: parent.width
            onClicked: {
                openInfoSheet(index);
            }

            contentItem: RowLayout {
                Layout.fillWidth: true

                Rectangle {
                    radius: height
                    Layout.preferredWidth: Kirigami.Units.largeSpacing
                    Layout.preferredHeight: Kirigami.Units.largeSpacing
                    color: Kirigami.Theme.neutralTextColor
                }

                Kirigami.IconTitleSubtitle {
                    Layout.fillWidth: true
                    title: key
                    subtitle: value
                    icon: icon.fromControlsIcon(subtitleDelegate.icon)
                }

                Controls.Button {
                    icon.name: "edit-entry"
                    onClicked: {
                        pageStack.push("qrc:AddEditAttributePage.qml", {
                            "entryItem": entryItem,
                            "entryAttribute": getEntryByIndex(index)
                        });
                    }
                }

            }

        }

        section.delegate: Kirigami.ListSectionHeader {
            width: parent.width

            Controls.ItemDelegate {
                text: section
            }

        }

    }

}
