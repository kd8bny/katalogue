// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.kitemmodels as KTM

import com.kd8bny.katalogue


Kirigami.ScrollablePage {
    id: itemsPage

    property bool isComponentView: false

    Layout.fillWidth: true

    title: i18n("Katalogued Items")

    header: Controls.ToolBar {
        id: toolbar
        RowLayout {
            anchors.fill: parent
            Kirigami.SearchField {
                id: searchField

                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                Layout.maximumWidth: Kirigami.Units.gridUnit*30
            }
        }
    }

    KTM.KSortFilterProxyModel {
        id: filteredModel
        sourceModel: ItemModel
        filterRoleName: "name"
        filterRegularExpression: {
            if (searchField.text === "") return new RegExp()
            return new RegExp("%1".arg(searchField.text), "i")
        }
    }

    actions: [
        Kirigami.Action {
            text: i18n("Add Item")
            icon.name: "list-add"
            tooltip: i18n("Add new item")
            onTriggered: pageStack.push("qrc:AddEditItemPage.qml")
        },
        Kirigami.Action {  //TODO radio button and undo
            text: i18n("Include Components")
            icon.name: "extension-symbolic"
            tooltip: i18n("Include Components")
            onTriggered: {
                ItemModel.filterComponent()  //TODO better name
            }
        }
    ]

    Component.onCompleted: {
        if(isComponentView){
            itemsLayout.model = ItemComponentModel
        }
    }

    Kirigami.CardsListView {
        id: itemsLayout
        model: filteredModel
        delegate: itemsDelegate

        Kirigami.PlaceholderMessage {
            anchors.centerIn: parent
            width: parent.width - (Kirigami.Units.largeSpacing * 4)

            visible: itemsLayout.count == 0
            text: i18n("Select 'Add Item' to start your katalogue.")
        }
    }

    Component {
        id: itemsDelegate

        Kirigami.AbstractCard {
            showClickFeedback: true
            onClicked: {
                pageStack.push("qrc:Details.qml", {"itemModelIndex": index, "itemId": id, "itemName": name, "isComponentView": isComponentView})
            }

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
                        //IMPORTANT: never put the bottom margin
                    }
                    rowSpacing: Kirigami.Units.largeSpacing
                    columnSpacing: Kirigami.Units.largeSpacing
                    columns: width > Kirigami.Units.gridUnit * 20 ? 4 : 2

                    Kirigami.Icon {
                        source: "file-catalog-symbolic"
                        Layout.fillHeight: true

                        Layout.fillWidth: false
                        Layout.maximumHeight: Kirigami.Units.iconSizes.huge
                        Layout.preferredWidth: height
                        Layout.rowSpan: 2
                    }

                    ColumnLayout {
                        Kirigami.Heading {
                            level: 1
                            text: name
                        }
                        Kirigami.Separator {
                            Layout.fillWidth: true
                        }
                        Controls.Label {
                            text: `${year} ${make} ${model}`
                        }
                    }

                    RowLayout {
                        Controls.Button {
                            icon.name: "arrow-up-symbolic"
                            visible: index == 0 ? false : true
                            onClicked: {
                                ItemModel.setItemPosition(index, -1)
                            }
                        }
                        Controls.Button {
                            icon.name: "arrow-down-symbolic"
                            visible: index == (ItemModel.rowCount() - 1) ? false : true
                            onClicked: {
                                ItemModel.setItemPosition(index, 1)
                            }
                        }
                    }
                }
            }
        }
    }
}
