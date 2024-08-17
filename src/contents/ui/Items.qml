// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import com.kd8bny.katalogue
import com.kd8bny.katalogue.entries
import org.kde.kirigami as Kirigami
import org.kde.kitemmodels as KTM

Kirigami.ScrollablePage {
    id: itemsPage

    property EntryItem entryComponent
    property bool isComponentView: false

    function getEntryByIndex(index) {
        var itemId = ItemModel.getId(index);
        if (isComponentView)
            itemId = ItemComponentModel.getId(index);

        return ItemDatabase.getEntryById(itemId);
    }

    Layout.fillWidth: true
    title: (isComponentView) ? i18n(entryComponent.name + " Components") : i18n("Katalogued Items")
    Component.onCompleted: {
        if (isComponentView)
            itemsLayout.model = ItemComponentModel;

    }
    actions: [
        Kirigami.Action {
            text: i18n("Add Item")
            icon.name: "list-add"
            tooltip: i18n("Add new item")
            onTriggered: pageStack.push("qrc:AddEditItemPage.qml")
        },
        //TODO radio button and undo
        Kirigami.Action {
            text: i18n("Include Components")
            icon.name: "extension-symbolic"
            tooltip: i18n("Include Components")
            onTriggered: {
                ItemModel.filterComponent(); //TODO better name
            }
        }
    ]

    KTM.KSortFilterProxyModel {
        id: filteredModel

        sourceModel: ItemModel
        filterRoleName: "name"
        filterRegularExpression: {
            if (searchField.text === "")
                return new RegExp();

            return new RegExp("%1".arg(searchField.text), "i");
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
                pageStack.push("qrc:Details.qml", {
                    "entryItem": getEntryByIndex(index)
                });
            }

            contentItem: Item {
                // implicitWidth/Height define the natural width/height of an item if no width or height is specified.
                // The setting below defines a component's preferred size based on its content
                implicitWidth: delegateLayout.implicitWidth
                implicitHeight: delegateLayout.implicitHeight

                GridLayout {
                    id: delegateLayout

                    rowSpacing: Kirigami.Units.largeSpacing
                    columnSpacing: Kirigami.Units.largeSpacing
                    columns: width > Kirigami.Units.gridUnit * 20 ? 4 : 2

                    anchors {
                        //IMPORTANT: never put the bottom margin

                        left: parent.left
                        top: parent.top
                        right: parent.right
                    }

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
                                ItemModel.setItemPosition(index, -1);
                            }
                        }

                        Controls.Button {
                            icon.name: "arrow-down-symbolic"
                            visible: index == (ItemModel.rowCount() - 1) ? false : true
                            onClicked: {
                                ItemModel.setItemPosition(index, 1);
                            }
                        }

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
            }

        }

    }

}
