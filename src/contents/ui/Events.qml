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

    Layout.fillWidth: true

    title: i18n("Events")

    actions {
        main: Kirigami.Action {
            text: i18n("Add")
            icon.name: "list-add"
            tooltip: i18n("Add new event")
            onTriggered: {
                pageStack.push("qrc:AddEditEventPage.qml", {"itemId": itemId})
            }
        }
        contextualActions: [
            Kirigami.Action {
                text: i18n("Attributes")
                icon.name: "item"
                tooltip: i18n("Swap to Attributes")
                onTriggered: pageStack.replace("qrc:Attributes.qml", {"itemName": itemName, "itemId": itemId})
            }
        ]
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
                        infoSheet.open({"eventIndex": index})
                    }
                },
                Kirigami.Action {
                    icon.name: "edit-entry"
                    onTriggered: {
                        pageStack.push("qrc:AddEditEventPage.qml", {
                            "itemId": itemId,
                            "isEdit": true,
                            "eventIndex": index,
                        })
                    }
                }
            ]

            RowLayout {
                id: delegateLayout

                Controls.Label {
                    wrapMode: Text.WordWrap
                    text: date
                }
                Controls.Label {
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                    text: event
                }
            }
        }
    }

    Kirigami.OverlaySheet {
        id: infoSheet

        header: Kirigami.Heading {
            text: "Event"
        }

        footer: Controls.Button {
            Layout.fillWidth: true
            text: i18nc("@action:button", "Close")
            onClicked: {
                infoSheet.close()
            }
        }

        Component.onCompleted: {
            var recordData = EventModel.getRecord(0)
            dateField.text = recordData[1]
            eventField.text = recordData[2]
            costField.text = recordData[3]
            odometerField.text = recordData[4]
            typeField = recordData[5]
            categoryField = recordData[6]
            commentField.text = recordData[7]
        }

        ColumnLayout {
            RowLayout {
                Layout.fillWidth: true
                Controls.Label {
                    id: dateField
                    wrapMode: Text.WordWrap
                }
                Controls.Label {
                    id: eventField
                    wrapMode: Text.WordWrap
                }
                Controls.Label {
                    id: costField
                    wrapMode: Text.WordWrap
                }
                Controls.Label {
                    id: odometerField
                    wrapMode: Text.WordWrap
                }
                Controls.Label {
                    id: commentField
                    wrapMode: Text.WordWrap
                }
                Controls.Label {
                    id: categoryField
                    wrapMode: Text.WordWrap
                }
                Controls.Label {
                    id: typeField
                    wrapMode: Text.WordWrap
                }
            }
        }
    }
}
