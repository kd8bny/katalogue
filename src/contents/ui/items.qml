// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami


Kirigami.ScrollablePage {
    id: itemsPage

    Layout.fillWidth: true

    title: i18n("Katalogued Items")

    Kirigami.CardsListView {
    id: layout
    model: itemModel
    delegate: itemsDelegate

    }

    Component {
        id: itemsDelegate
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

                    Kirigami.Heading {
                        Layout.fillHeight: true
                        level: 1
                        text: NAME
                    }

                    ColumnLayout {
                        Kirigami.Heading {
                            Layout.fillWidth: true
                            level: 2
                            text: YEAR
                        }
                        Kirigami.Separator {
                            Layout.fillWidth: true
                            //visible: description.length > 0
                        }
                        Controls.Label {
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                            text: MODEL + MAKE
                            //visible: description.length > 0
                        }
                    }
                    Controls.Button {
                        Layout.alignment: Qt.AlignRight
                        Layout.columnSpan: 2
                        text: i18n("Edit")
                        // onClicked: to be done... soon!
                    }
                }
            }
        }
    }
}