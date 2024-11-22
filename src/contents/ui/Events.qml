// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import com.kd8bny.katalogue
import com.kd8bny.katalogue.entries
import org.kde.kirigami as Kirigami

Kirigami.ScrollablePage {
    id: eventsPage

    required property EntryItem entryItem
    property EventModel eventModel

    function getEntryByIndex(index) {
        return EventDatabase.getEntryById(EventModel.getId(index));
    }

    function openInfoSheet(index = -1) {
        var event = getEntryByIndex(index);
        eventInfoSheet.date = event.date.split("T")[0];
        eventInfoSheet.event = event.event;
        eventInfoSheet.cost = event.cost;
        eventInfoSheet.increment = event.increment;
        eventInfoSheet.category = event.category;
        eventInfoSheet.comment = event.comment;
        eventInfoSheet.open();
    }

    Layout.fillWidth: true
    title: i18n(entryItem.name + " Events")
    Component.onCompleted: {
        eventModel = EventModel;
        eventModel.setItemId(entryItem.id);
    }
    actions: [
        Kirigami.Action {
            text: i18n("Add")
            icon.name: "list-add"
            tooltip: i18n("Add new event")
            onTriggered: {
                pageStack.push("qrc:AddEditEventPage.qml", {
                    "entryItem": entryItem
                });
            }
        },
        Kirigami.Action {
            text: i18n("Sort By")
            icon.name: "extension-symbolic"
            tooltip: i18n("")

            Kirigami.Action {
                text: i18n("Default")
                icon.name: "extension-symbolic"
                onTriggered: {
                    eventModel.setSortRole(EventModel.SortRole.DEFAULT);
                }
            }

            Kirigami.Action {
                text: i18n("Event")
                icon.name: "extension-symbolic"
                onTriggered: {
                    eventModel.setSortRole(EventModel.SortRole.EVENT);
                }
            }

            Kirigami.Action {
                text: i18n("Category")
                icon.name: "extension-symbolic"
                onTriggered: {
                    eventModel.setSortRole(EventModel.SortRole.CATEGORY);
                }
            }

            Kirigami.Action {
                separator: true
            }

            Kirigami.Action {
                text: i18n("Sort ASC")
                icon.name: "extension-symbolic"
                onTriggered: {
                    eventModel.setSortOrder(0);
                }
            }

            Kirigami.Action {
                text: i18n("Sort DESC")
                icon.name: "extension-symbolic"
                checked: true
                onTriggered: {
                    eventModel.setSortOrder(1);
                }
            }

        }
    ]

    ListView {
        id: layout

        model: eventModel.getFilterProxyModel()
        focus: true

        Kirigami.PlaceholderMessage {
            anchors.centerIn: layout
            width: layout.width - (Kirigami.Units.largeSpacing * 4)
            visible: layout.count == 0
            text: i18n("Select add to add an event to this item")
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
                    Layout.preferredWidth: Kirigami.Units.largeSpacing * 30
                    title: event
                    subtitle: date.split("T")[0]
                    icon: icon.fromControlsIcon(subtitleDelegate.icon)
                }

                Controls.Button {
                    icon.name: "edit-entry"
                    onClicked: {
                        pageStack.push("qrc:AddEditEventPage.qml", {
                            "entryItem": entryItem,
                            "entryEvent": getEntryByIndex(index)
                        });
                    }
                }

            }

        }

    }

    header: Controls.ToolBar {
        id: toolbar

        RowLayout {
            anchors.fill: parent

            Kirigami.SearchField {
                id: searchField

                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                Layout.maximumWidth: Kirigami.Units.gridUnit * 30
                onTextChanged: {
                    eventModel.getFilterProxyModel().setFilterString(text);
                }
            }

        }

    }

}
