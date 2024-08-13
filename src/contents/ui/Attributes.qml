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
        delegate: attributeDelegate
        section.property: "category"
        section.delegate: sectionDelegate
        focus: true
        headerPositioning: ListView.OverlayHeader

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
                // TODO icon: icon.fromControlsIcon(subtitleDelegate.icon)

                Layout.fillWidth: true
                title: section
            }

        }

    }

    Component {
        id: attributeDelegate

        Kirigami.SubtitleDelegate {
            id: subtitleDelegate

            Layout.fillWidth: true
            Layout.preferredWidth: Kirigami.Units.largeSpacing * 60
            text: key
            subtitle: value

            contentItem: RowLayout {
                Layout.fillWidth: true

                Rectangle {
                    Layout.fillWidth: true
                    radius: height
                    Layout.preferredWidth: Kirigami.Units.largeSpacing
                    Layout.preferredHeight: Kirigami.Units.largeSpacing
                    color: Kirigami.Theme.neutralTextColor
                }

                Kirigami.IconTitleSubtitle {
                    Layout.fillWidth: true
                    Layout.preferredWidth: Kirigami.Units.largeSpacing * 30
                    title: subtitleDelegate.text
                    subtitle: subtitleDelegate.subtitle
                    icon: icon.fromControlsIcon(subtitleDelegate.icon)
                }

                Controls.Button {
                    icon.name: "kdocumentinfo"
                    onClicked: {
                        openInfoSheet(index);
                    }
                }

                Controls.Button {
                    Layout.fillWidth: true
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

    }

}
