// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import com.kd8bny.katalogue
import com.kd8bny.katalogue.entries
import org.kde.kirigami as Kirigami

Kirigami.ScrollablePage {
    id: notesPage

    property EntryItem entryItem
    property NoteModel noteModel

    function getEntryByIndex(index) {
        return NoteDatabase.getEntryById(NoteModel.getId(index));
    }

    title: (entryItem) ? i18n(entryItem.name + " Notes") : i18n("All Notes")
    Component.onCompleted: {
        noteModel = NoteModel;
        if (entryItem)
            noteModel.setItemId(entryItem.id);

    }
    actions: [
        Kirigami.Action {
            text: i18n("Add Note")
            icon.name: "list-add"
            tooltip: i18n("Add new note")
            onTriggered: {
                pageStack.push("qrc:AddEditNotePage.qml", {
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
                    noteModel.setSortRole(NoteModel.SortRole.DEFAULT);
                }
            }

            Kirigami.Action {
                text: i18n("Note Content")
                icon.name: "extension-symbolic"
                onTriggered: {
                    noteModel.setSortRole(NoteModel.SortRole.NOTE_CONTENT);
                }
            }

            Kirigami.Action {
                separator: true
            }

            Kirigami.Action {
                text: i18n("Sort ASC")
                icon.name: "extension-symbolic"
                onTriggered: {
                    noteModel.setSortOrder(0);
                }
            }

            Kirigami.Action {
                text: i18n("Sort DESC")
                icon.name: "extension-symbolic"
                checked: true
                onTriggered: {
                    noteModel.setSortOrder(1);
                }
            }

        }
    ]

    Kirigami.CardsListView {
        id: layout

        model: noteModel.getFilterProxyModel()
        delegate: notesDelegate
        headerPositioning: ListView.OverlayHeader

        Kirigami.PlaceholderMessage {
            anchors.centerIn: layout
            width: layout.width - (Kirigami.Units.largeSpacing * 4)
            visible: layout.count == 0
            text: i18n("Select 'Add Note' to insert a new Note")
        }

    }

    Component {
        id: notesDelegate

        Kirigami.Card {
            showClickFeedback: true
            onClicked: {
                pageStack.push("qrc:AddEditNotePage.qml", {
                    "entryItem": entryItem,
                    "entryNote": getEntryByIndex(index)
                });
            }

            banner {
                //TODO clean up view
                // source: "../banner.jpg"
                title: title
                // The title can be positioned in the banner
                titleAlignment: Qt.AlignLeft | Qt.AlignBottom
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

                    Controls.Label {
                        wrapMode: TextEdit.Wrap
                        text: note_content
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
                    noteModel.getFilterProxyModel().setFilterString(text);
                }
            }

        }

    }

}
