#include "noteModel.h"


NoteModel::NoteModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    QObject::connect(this, SIGNAL(dataChanged()), this, SLOT(refresh()));

    this->refresh();
}

NoteModel::~NoteModel()
{

}

// The method for obtaining data from the model
QVariant NoteModel::data(const QModelIndex & index, int role) const {

    // Define the column number, on the role of number
    int columnId = role - Qt::UserRole - 1;
    // Create the index using a column ID
    QModelIndex modelIndex = this->index(index.row(), columnId);

    return QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
}

QHash<int, QByteArray> NoteModel::roleNames() const {

    QHash<int, QByteArray> roles;
    roles[rID] = "id";
    roles[rTitle] = Database::TITLE.toUtf8();
    roles[rNote] = Database::NOTE.toUtf8();
    roles[rItemID] = Database::KEY_ITEM_ID.toUtf8();

    return roles;
}

void NoteModel::setItemId(QString itemId)
{
    this->modelQuery = this->modelQueryBase + this->modelQuerySetId.arg(
        Database::KEY_ITEM_ID, itemId);

    this->setQuery(modelQuery);
    Q_EMIT dataChanged();
}

void NoteModel::refresh()
{
    this->setQuery(this->modelQueryBase);
    qDebug()<< this->modelQueryBase;
}

int NoteModel::getId(int row)
{
    return this->data(this->index(row, 0), rID).toInt();
}

Note NoteModel::getRecord(int row)
{
    int id = this->data(this->index(row, 0), rID).toInt();
    Note note(id);

    note.setTitle(this->data(this->index(row, 1), rTitle).toString());
    note.setNoteContent(this->data(this->index(row, 2), rNote).toString());
    note.setItemId(this->data(this->index(row, 4), rItemID).toInt());

    return note;
}

QVariantList NoteModel::getRecordAsList(int row)
{
    Note note = this->getRecord(row);

    return note.asList();
}

bool NoteModel::setRecord(int noteIndex, QString title, QString noteContent, int itemId)
{
    Database db;
    // noteIndex defaults to -1 for new entries.
    Note note(this->getId(noteIndex));

    bool success = false;

    note.setTitle(title);
    note.setNoteContent(noteContent);
    note.setItemId(itemId);

    qDebug() << note.asList();

    if (noteIndex == -1) {
        success = db.insertNoteEntry(note);
        qDebug() << "noteModel | Inserting Entry | " << success;
    } else {
        success = db.updateNoteEntry(note);
        qDebug() << "noteModel | Updating Entry | " << success;
    }

    if (success)
        Q_EMIT dataChanged();

    return success;

}

bool NoteModel::deleteRecord(int noteId)
{
    Database db;

    bool success = false;

    success = db.deleteNoteEntry(noteId);

    if (success)
        Q_EMIT dataChanged();

    return success;
}
