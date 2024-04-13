// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import com.kd8bny.katalogue


Kirigami.ScrollablePage {
    id: page

    required property int itemId
    required property string itemName

    property int eventIndex

    Layout.fillWidth: true

    title: i18n("Events")

    actions: [
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

        Kirigami.SubtitleDelegate {
            id: subtitleDelegate
            Layout.fillWidth: true

            text: event
            subtitle: date.split("T")[0]

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
                        pageStack.push("qrc:AddEditEventPage.qml",
                            {"eventIndex": index, "itemId": itemId})
                    }
                }
            }
        }
        // Kirigami.SwipeListItem {
        //     separatorVisible: true

        //     actions: [
        //         Kirigami.Action {
        //             icon.name: "kdocumentinfo"
        //             onTriggered: {
        //                 openInfoSheet(index)
        //             }
        //         },
        //         Kirigami.Action {
        //             icon.name: "edit-entry"
        //             onTriggered: {
        //                 pageStack.push("qrc:AddEditEventPage.qml", {"eventIndex": index, "itemId": itemId})
        //             }
        //         }
        //     ]

        //     RowLayout {
        //         id: delegateLayout

        //         Controls.Label {
        //             wrapMode: Text.WordWrap
        //             text: date.split("T")[0]
        //         }
        //         Controls.Label {
        //             Layout.fillWidth: true
        //             wrapMode: Text.WordWrap
        //             text: event
        //         }
        //     }
        // }
    }
}
