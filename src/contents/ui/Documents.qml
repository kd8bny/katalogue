// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import com.kd8bny.katalogue
import com.kd8bny.katalogue.entries
import org.kde.kirigami as Kirigami

Kirigami.ScrollablePage {
    id: documentsPage

    property EntryItem entryItem
    property EntryDocument entryDocument
    property DocumentModel documentModel

    function getEntryByIndex(index) {
        return DocumentDatabase.getEntryById(DocumentModel.getId(index));
    }

    Layout.fillWidth: true
    title: (entryItem) ? i18n(entryItem.name + " Documents") : i18n("All Documents")
    Component.onCompleted: {
        documentModel = DocumentModel;
        if (entryItem)
            documentModel.setItemId(entryItem.id);

    }
    actions: [
        Kirigami.Action {
            text: i18n("Add")
            icon.name: "list-add"
            tooltip: i18n("Add new document")
            onTriggered: {
                pageStack.push("qrc:AddEditDocumentPage.qml", {
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
                    documentModel.setSortRole(DocumentModel.SortRole.DEFAULT);
                }
            }

            Kirigami.Action {
                text: i18n("Filename")
                icon.name: "extension-symbolic"
                onTriggered: {
                    documentModel.setSortRole(DocumentModel.SortRole.FILENAME);
                }
            }

            Kirigami.Action {
                separator: true
            }

            Kirigami.Action {
                text: i18n("Sort ASC")
                icon.name: "extension-symbolic"
                onTriggered: {
                    documentModel.setSortOrder(0);
                }
            }

            Kirigami.Action {
                text: i18n("Sort DESC")
                icon.name: "extension-symbolic"
                checked: true
                onTriggered: {
                    documentModel.setSortOrder(1);
                }
            }

        }
    ]

    FileDialog {
        id: fileSaveDialog

        currentFolder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)[0]
        fileMode: FileDialog.SaveFile
        selectedFile: entryDocument.fileName
        onAccepted: {
            fileSaveDialog.selectedFile = entryDocument.fileName;
            DocumentIOHelper.writeFile(fileSaveDialog.currentFolder, entryDocument);
        }
    }

    ListView {
        id: layout

        model: documentModel.getFilterProxyModel()
        focus: true

        Kirigami.PlaceholderMessage {
            anchors.centerIn: layout
            width: layout.width - (Kirigami.Units.largeSpacing * 4)
            visible: layout.count == 0
            text: i18n("Select add to add an document to this item")
        }

        delegate: Kirigami.SubtitleDelegate {
            id: subtitleDelegate

            width: parent.width
            text: name
            subtitle: file_name
            onClicked: {
                Qt.openUrlExternally(DocumentIOHelper.writeFile("", getEntryByIndex(index)));
            }

            contentItem: RowLayout {
                Layout.fillWidth: true

                Rectangle {
                    radius: height
                    Layout.preferredWidth: Kirigami.Units.largeSpacing
                    Layout.preferredHeight: Kirigami.Units.largeSpacing
                    color: Kirigami.Theme.neutralTextColor
                }

                Kirigami.IconTitleSubtitle {
                    Layout.fillWidth: true
                    Layout.preferredWidth: Kirigami.Units.largeSpacing * 30
                    title: name
                    subtitle: file_name
                    icon: icon.fromControlsIcon(subtitleDelegate.icon)
                }

                Controls.Button {
                    icon.name: "document-open"
                    onClicked: {
                        Qt.openUrlExternally(DocumentIOHelper.writeFile("", getEntryByIndex(index)));
                    }
                }

                Controls.Button {
                    icon.name: "download-symbolic"
                    onClicked: {
                        entryDocument = getEntryByIndex(index);
                        fileSaveDialog.open();
                    }
                }

                Controls.Button {
                    icon.name: "edit-entry"
                    onClicked: {
                        pageStack.push("qrc:AddEditDocumentPage.qml", {
                            "entryItem": entryItem,
                            "entryDocument": getEntryByIndex(index)
                        });
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
                    documentModel.getFilterProxyModel().setFilterString(text);
                }
            }

        }

    }

}
