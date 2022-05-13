// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import com.kd8bny.katalogue 1.0


Kirigami.ScrollablePage {
    id: attributesPage

    // required property string itemName

    Layout.fillWidth: true

    title: i18n("@itemName")

    actions {
        main: Kirigami.Action {
            text: i18n("Add")
            icon.name: "list-add"
            tooltip: i18n("Add new attribute")
            onTriggered: addAttributeSheet.open()
        }
    }

    Kirigami.CardsListView {
        id: layout
        model: AttributeModel
        delegate: attributeDelegate
    }

    Component {
        id: attributeDelegate

        Kirigami.AbstractCard {
            contentItem: Item {
                // implicitWidth/Height define the natural width/height of an item if no width or height is specified.
                // The setting below defines a component's preferred size based on its content
                implicitWidth: delegateLayout.implicitWidth
                implicitHeight: delegateLayout.implicitHeight
                GridLayout {
                    id: delegateLayout
                    anchors {
                        left: parent.left
                        top: parent.top
                        right: parent.right
                    }
                    rowSpacing: Kirigami.Units.largeSpacing
                    columnSpacing: Kirigami.Units.largeSpacing
                    columns: root.wideScreen ? 4 : 2

                    ColumnLayout {
                        Kirigami.Heading {
                            Layout.fillWidth: true
                            level: 2
                            text: KEY
                        }
                        Kirigami.Heading {
                            Layout.fillWidth: true
                            level: 2
                            text: VALUE
                        }
                    }
                    Controls.Button {
                        Layout.alignment: Qt.AlignRight
                        // Layout.alignment: Qt.AlignBottom
                        Layout.columnSpan: 2
                        text: i18n("Edit")
                        // onClicked: to be done... soon!
                    }
                }
            }
        }
    }
}
