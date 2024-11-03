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
    id: addEditDocumentPage

    property EntryItem entryItem
    property EntryDocument entryDocument
    property bool isEdit: false
    property string selectedFileUrl: ""

    signal modelModified()

    function insertUpdate() {
        entryDocument.name = nameField.text;
        if (entryItem)
            entryDocument.itemId = entryItem.id;

        if (selectedFileUrl)
            DocumentIOHelper.loadFile(selectedFileUrl, entryDocument);

        if (isEdit)
            return DocumentDatabase.updateEntry(entryDocument);
        else
            return DocumentDatabase.insertEntry(entryDocument);
        return false;
    }

    function setFileNameAndURL(path) {
        var filePathArray = path.toString().slice(7).split("/");
        entryDocument.fileName = filePathArray.pop();
        selectedFileUrl = path;
        filePathField.text = selectedFileUrl;
        filePathField.enabled = true;
    }

    title: (isEdit) ? i18n("Edit Document") : i18n("Add Document")
    Component.onCompleted: {
        if (entryDocument) {
            isEdit = true;
            nameField.text = entryDocument.name;
            filePathField.text = entryDocument.fileName;
        } else {
            entryDocument = DocumentDatabase.getEntryById();
        }
    }
    actions: [
        Kirigami.Action {
            visible: isEdit
            text: i18n("Delete")
            icon.name: "delete"
            tooltip: i18n("Remove document from Katalogue")
            onTriggered: {
                deleteDialog.open();
            }
        }
    ]

    Controls.Action {
        id: addUpdateAction

        shortcut: "Return"
        onTriggered: {
            if (insertUpdate()) {
                DocumentModel.refreshModel();
                pageStack.pop();
            } else {
                msgInsertUpdateError.visible = true;
            }
        }
    }

    Kirigami.PromptDialog {
        id: deleteDialog

        title: i18n("Delete")
        subtitle: i18n("Are you sure you want to delete Document")
        standardButtons: Kirigami.Dialog.Ok | Kirigami.Dialog.Cancel
        onAccepted: {
            if (DocumentDatabase.deleteEntryById(entryDocument.id)) {
                DocumentModel.refreshModel();
                pageStack.clear();
                pageStack.push("qrc:Documents.qml");
            } else {
                msgDeleteError.visible = true;
            }
        }
        onRejected: {
            pageStack.pop();
        }
    }

    FileDialog {
        id: fileDialog

        currentFolder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)[0]
        onAccepted: {
            setFileNameAndURL(selectedFile);
        }
    }

    ColumnLayout {
        Kirigami.InlineMessage {
            id: msgInsertUpdateError

            Layout.fillWidth: true
            type: Kirigami.MessageType.Error
            text: (isEdit) ? i18n("Failed to update Document") : i18n("Failed to insert Document")
            visible: false
        }

        Kirigami.InlineMessage {
            id: msgDeleteError

            Layout.fillWidth: true
            type: Kirigami.MessageType.Error
            text: "Failed to delete Document"
            visible: false
        }

        Kirigami.FormLayout {
            id: form

            Controls.TextField {
                id: nameField

                Kirigami.FormData.label: i18nc("@label:textbox", "Name:")
                placeholderText: i18n("Required")
                enabled: !entryDocument.archived
            }

            Kirigami.Separator {
                Kirigami.FormData.isSection: true
            }

            RowLayout {
                Kirigami.FormData.label: i18nc("@label:textbox", "Document:")

                Controls.TextField {
                    id: filePathField

                    placeholderText: i18n("Select browse")
                    enabled: !entryDocument.archived & false
                    onTextEdited: {
                        setFileNameAndURL(filePathField.text);
                    }
                }

                Controls.Button {
                    text: i18n("Browse")
                    icon.name: 'document-open-symbolic'
                    onClicked: fileDialog.open()
                }

            }

        }

    }

    footer: Controls.DialogButtonBox {
        standardButtons: Controls.DialogButtonBox.Cancel
        onRejected: pageStack.pop()
        onAccepted: addUpdateAction.trigger()

        Controls.Button {
            Controls.DialogButtonBox.buttonRole: Controls.DialogButtonBox.AcceptRole
            enabled: nameField.length > 0
            icon.name: isEdit ? "document-save" : "list-add"
            text: isEdit ? i18n("Save") : i18n("Add")
        }

    }

}
