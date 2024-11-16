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

    property EntryItem entryItem
    property var itemModelType: ItemModel.ITEMS
    property ItemModel itemModel

    function getEntryByIndex(index) {
        var itemId = itemModel.getId(index);
        return ItemDatabase.getEntryById(itemId);
    }

    Layout.fillWidth: true
    title: i18n("Katalogued Items")
    Component.onCompleted: {
        if (itemModelType == ItemModel.ITEMS) {
            itemModel = ItemModelFactory.createItemModel();
        } else if (itemModelType == ItemModel.COMPONENTS) {
            itemsPage.title = i18n(entryItem.name + " Components");
            itemModel = ItemModelFactory.createItemComponentModel(entryItem.id);
        } else if (itemModelType == ItemModel.ARCHIVE) {
            itemsPage.title = i18n("Archive");
            itemModel = ItemModelFactory.createItemArchivedModel();
        }
    }
    actions: [
        Kirigami.Action {
            text: i18n("Add Item")
            icon.name: "list-add"
            tooltip: i18n("Add new item")
            onTriggered: pageStack.push("qrc:AddEditItemPage.qml")
        },
        Kirigami.Action {
            text: i18n("Sort By")
            icon.name: "extension-symbolic"
            tooltip: i18n("")

            Kirigami.Action {
                text: i18n("Default")
                icon.name: "extension-symbolic"
                onTriggered: {
                    itemModel.setSortRole(ItemModel.SortRole.DEFAULT);
                }
            }

            Kirigami.Action {
                text: i18n("Name")
                icon.name: "extension-symbolic"
                onTriggered: {
                    itemModel.setSortRole(ItemModel.SortRole.NAME);
                }
            }

            Kirigami.Action {
                text: i18n("Type")
                icon.name: "extension-symbolic"
                onTriggered: {
                    itemModel.setSortRole(ItemModel.SortRole.TYPE);
                }
            }

            Kirigami.Action {
                text: i18n("Year")
                icon.name: "extension-symbolic"
                onTriggered: {
                    itemModel.setSortRole(ItemModel.SortRole.YEAR);
                }
            }

            Kirigami.Action {
                separator: true
            }

            Kirigami.Action {
                text: i18n("Sort ASC")
                icon.name: "extension-symbolic"
                onTriggered: {
                    itemModel.setSortOrder(0);
                }
            }

            Kirigami.Action {
                text: i18n("Sort DESC")
                icon.name: "extension-symbolic"
                checked: true
                onTriggered: {
                    itemModel.setSortOrder(1);
                }
            }

        },
        Kirigami.Action {
            text: i18n("Show Components")
            icon.name: "extension-symbolic"
            tooltip: i18n("")
            checkable: true
            visible: itemModelType != ItemModel.ItemModelType.COMPONENTS
            onToggled: {
                itemModel.toggleComponents();
            }
        }
    ]

    Kirigami.CardsListView {
        id: itemsLayout

        model: itemModel.getFilterProxyModel()
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
                        source: fk_item_id ? "extension-symbolic" : "file-catalog-symbolic"
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
                            text: `${year} ${make} ${model} ${name}`
                        }

                    }

                    RowLayout {
                        Controls.Button {
                            icon.name: "arrow-up-symbolic"
                            visible: index == 0 ? false : true
                            onClicked: {
                                itemModel.setItemPosition(index, -1);
                            }
                        }

                        Controls.Button {
                            icon.name: "arrow-down-symbolic"
                            visible: index == (itemModel.rowCount() - 1) ? false : true
                            onClicked: {
                                itemModel.setItemPosition(index, 1);
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
                onTextChanged: {
                    itemModel.getFilterProxyModel().setFilterString(text);
                }
            }

        }

    }

}
