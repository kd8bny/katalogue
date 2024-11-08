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

    function getEntryByIndex(index) {
        return DocumentDatabase.getEntryById(DocumentModel.getId(index));
    }

    Layout.fillWidth: true
    title: (entryItem) ? i18n(entryItem.name + " Documents") : i18n("All Documents")
    Component.onCompleted: {
        if (entryItem)
            DocumentModel.setItemIdQuery(entryItem.id);

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

        model: DocumentModel
        delegate: documentDelegate
        focus: true

        Kirigami.PlaceholderMessage {
            anchors.centerIn: layout
            width: layout.width - (Kirigami.Units.largeSpacing * 4)
            visible: layout.count == 0
            text: i18n("Select add to add an document to this item")
        }

    }

    Component {
        id: documentDelegate

        Kirigami.SubtitleDelegate {
            id: subtitleDelegate

            Layout.fillWidth: true
            text: name
            subtitle: file_name

            contentItem: RowLayout {
                Layout.fillWidth: true

                Rectangle {
                    Layout.fillWidth: true
                    radius: height
                    Layout.preferredWidth: Kirigami.Units.largeSpacing
                    Layout.preferredHeight: Kirigami.Units.largeSpacing
                    color: Kirigami.Theme.neutralTextColor
                }

                Kirigami.IconTitleSubtitle {
                    Layout.fillWidth: true
                    Layout.preferredWidth: Kirigami.Units.largeSpacing * 30
                    title: subtitleDelegate.text
                    subtitle: subtitleDelegate.subtitle
                    icon: icon.fromControlsIcon(subtitleDelegate.icon)
                }

                Controls.Button {
                    Layout.fillWidth: true
                    icon.name: "document-open"
                    onClicked: {
                        Qt.openUrlExternally(DocumentIOHelper.writeFile("", getEntryByIndex(index)));
                    }
                }

                Controls.Button {
                    Layout.fillWidth: true
                    icon.name: "download-symbolic"
                    onClicked: {
                        entryDocument = getEntryByIndex(index);
                        fileSaveDialog.open();
                    }
                }

                Controls.Button {
                    Layout.fillWidth: true
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

}
