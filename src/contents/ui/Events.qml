// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami
import com.kd8bny.katalogue 1.0


Kirigami.ScrollablePage {
    id: page

    required property int itemId
    required property string itemName

    property int eventIndex

    Layout.fillWidth: true

    title: i18n("Events")

    actions.contextualActions: [
        Kirigami.Action {
            text: i18n("Add")
            icon.name: "list-add"
            tooltip: i18n("Add new event")
            onTriggered: {
                pageStack.push("qrc:AddEditEventPage.qml", {"itemId": itemId})
            }
        }
    ]

    function openInfoSheet(index = -1) {
        var recordData = EventModel.getRecordAsList(index)

        eventInfoSheet.date = recordData[3].split("T")[0]
        eventInfoSheet.event = recordData[4]
        eventInfoSheet.cost = recordData[5]
        eventInfoSheet.odometer = recordData[6]
        eventInfoSheet.category = recordData[7]
        eventInfoSheet.comment = recordData[8]

        eventInfoSheet.open()
    }


    Component.onCompleted: EventModel.setItemId(itemId)

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
                        pageStack.push("qrc:AddEditEventPage.qml", {"eventIndex": index, "itemId": itemId})
                    }
                }
            ]

            RowLayout {
                id: delegateLayout

                Controls.Label {
                    wrapMode: Text.WordWrap
                    text: date.split("T")[0]
                }
                Controls.Label {
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                    text: event
                }
            }
        }
    }
}
