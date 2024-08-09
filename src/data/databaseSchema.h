#include <QString>

#ifndef DATABASE_SCHEMA_H
#define DATABASE_SCHEMA_H

namespace DatabaseSchema
{
     const int DATABASE_VERSION = 1;
     const QString DATABASE_TYPE = QStringLiteral("QSQLITE");
     const QString DATABASE_NAME = QStringLiteral("katalogue.db");

     const QString TABLE_ATTRIBUTES = QStringLiteral("attributes");
     const QString TABLE_EVENTS = QStringLiteral("events");
     const QString TABLE_ITEMS = QStringLiteral("items");
     const QString TABLE_NOTES = QStringLiteral("notes");
     const QString TABLE_TASKS = QStringLiteral("tasks");
     const QString TABLE_DEFAULTS = QStringLiteral("defaults");

     const QString ARCHIVED = QStringLiteral("archived");
     const QString CATEGORY = QStringLiteral("category");
     const QString COMMENT = QStringLiteral("comment");
     const QString COST = QStringLiteral("cost");
     const QString CREATED = QStringLiteral("created");
     const QString DATE = QStringLiteral("date");
     const QString GROUP = QStringLiteral("item_group");
     const QString KEY = QStringLiteral("key");
     const QString LABEL = QStringLiteral("label");
     const QString EVENT = QStringLiteral("event");
     const QString MAKE = QStringLiteral("make");
     const QString MODEL = QStringLiteral("model");
     const QString MODIFIED = QStringLiteral("modified");
     const QString NAME = QStringLiteral("name");
     const QString USER_ORDER = QStringLiteral("user_order");
     const QString TYPE = QStringLiteral("type");
     const QString VALUE = QStringLiteral("value");
     const QString YEAR = QStringLiteral("year");
     const QString INCREMENT = QStringLiteral("increment");
     const QString TITLE = QStringLiteral("title");
     const QString NOTE_CONTENT = QStringLiteral("note_content");
     const QString DESCRIPTION = QStringLiteral("description");
     const QString DUE_DATE = QStringLiteral("due_date");
     const QString STATUS = QStringLiteral("status");

     const QString KEY_ITEM_ID = QStringLiteral("fk_item_id");

     const QString SCHEMA_ITEMS = QStringLiteral("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                 "%2 DATE NOT NULL, "                          // CREATED
                                                 "%3 DATE NOT NULL, "                          // MODIFIED
                                                 "%4 TEXT NOT NULL, "                          // NAME
                                                 "%5 TEXT, "                                   // MAKE
                                                 "%6 TEXT, "                                   // MODEL
                                                 "%7 INT, "                                    // YEAR
                                                 "%8 TEXT NOT NULL, "                          // TYPE
                                                 "%9 BOOLEAN NOT NULL CHECK ( %9 IN (0, 1)), " // ARCHIVED
                                                 "%10 INT, "                                   // USER_ORDER
                                                 "%11 INT, "                                   // FK
                                                 "FOREIGN KEY (%11) REFERENCES %1 (id) "
                                                 "ON DELETE CASCADE ON UPDATE CASCADE)")
                                      .arg(TABLE_ITEMS, CREATED, MODIFIED, NAME, MAKE, MODEL, YEAR, TYPE, ARCHIVED,
                                           USER_ORDER, KEY_ITEM_ID);

     const QString SCHEMA_ATTRIBUTES = QStringLiteral("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                      "%2 DATE NOT NULL, " // CREATED
                                                      "%3 DATE NOT NULL, " // Modified
                                                      "%4 TEXT NOT NULL, " // KEY
                                                      "%5 TEXT NOT NULL, " // VALUE
                                                      "%6 TEXT, "          // CATEGORY
                                                      "%7 INT NOT NULL, "  // FK
                                                      "CONSTRAINT %7 FOREIGN KEY (%7) REFERENCES %8 (id) "
                                                      "ON DELETE CASCADE ON UPDATE CASCADE)")
                                           .arg(TABLE_ATTRIBUTES, CREATED, MODIFIED, KEY, VALUE, CATEGORY, KEY_ITEM_ID,
                                                TABLE_ITEMS);

     const QString SCHEMA_EVENTS = QStringLiteral("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                  "%2 DATE NOT NULL, "             // CREATED
                                                  "%3 DATE NOT NULL, "             // MODIFIED
                                                  "%4 DATE NOT NULL, "             // DATE
                                                  "%5 TEXT NOT NULL, "             // EVENT
                                                  "%6 REAL NOT NULL DEFAULT 0, "   // COST
                                                  "%7 REAL NOT NULL DEFAULT 0.0, " // INCREMENT
                                                  "%8 TEXT, "                      // CATEGORY
                                                  "%9 VARCHAR(255), "              // COMMENT
                                                  "%10 INT NOT NULL, "             // FK
                                                  "CONSTRAINT %10 FOREIGN KEY (%10) REFERENCES %11 (id) "
                                                  "ON DELETE CASCADE ON UPDATE CASCADE)")
                                       .arg(TABLE_EVENTS, CREATED, MODIFIED, DATE, EVENT, COST, INCREMENT, CATEGORY,
                                            COMMENT, KEY_ITEM_ID, TABLE_ITEMS);

     const QString SCHEMA_NOTES = QStringLiteral("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                 "%2 DATE NOT NULL, "                   // CREATED
                                                 "%3 DATE NOT NULL, "                   // MODIFIED
                                                 "%4 TEXT NOT NULL, "                   // TITLE
                                                 "%5 VARCHAR(255), "                    // Note Content
                                                 "%6 INT, "                             // FK
                                                 "FOREIGN KEY (%6) REFERENCES %7 (id) " // Notes and tasks do not need to belong to item
                                                 "ON DELETE CASCADE ON UPDATE CASCADE)")
                                      .arg(TABLE_NOTES, CREATED, MODIFIED, TITLE, NOTE_CONTENT, KEY_ITEM_ID, TABLE_ITEMS);

     const QString SCHEMA_TASKS = QStringLiteral("CREATE TABLE %1 (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                                 "%2 DATE NOT NULL, "                   // CREATED
                                                 "%3 DATE NOT NULL, "                   // MODIFIED
                                                 "%4 DATE NOT NULL, "                   // DUE_DATE
                                                 "%7 TEXT NOT NULL, "                   // STATUS
                                                 "%5 TEXT NOT NULL, "                   // TITLE
                                                 "%6 VARCHAR(255), "                    // DESCRIPTION
                                                 "%8 INT, "                             // FK
                                                 "FOREIGN KEY (%8) REFERENCES %9 (id) " // Notes and tasks do not need to belong to item
                                                 "ON DELETE CASCADE ON UPDATE CASCADE)")
                                      .arg(TABLE_TASKS, CREATED, MODIFIED, DUE_DATE, STATUS, TITLE, DESCRIPTION,
                                           KEY_ITEM_ID, TABLE_ITEMS);
};

#endif