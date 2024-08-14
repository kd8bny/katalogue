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
    Component.onCompleted: EventModel.setItemIdQuery(entryItem.id)
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
        }
    ]

    ListView {
        id: layout

        model: EventModel
        delegate: eventDelegate
        focus: true

        Kirigami.PlaceholderMessage {
            anchors.centerIn: layout
            width: layout.width - (Kirigami.Units.largeSpacing * 4)
            visible: layout.count == 0
            text: i18n("Select add to add an event to this item")
        }

    }

    Component {
        id: eventDelegate

        Kirigami.SubtitleDelegate {
            id: subtitleDelegate

            Layout.fillWidth: true
            text: event
            subtitle: date.split("T")[0]

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

}
