# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

set(katalogue_entries_src
    entry.h
    item.h
    attribute.h
    event.h
    note.h
    task.h
)

add_library(katalogue_entries STATIC
    ${katalogue_entries_src}
)

target_link_libraries(katalogue_entries
    Qt6::Core
    Qt6::Qml
)

qt_add_qml_module(entries
  URI com.kd8bny.katalogue.entries
  VERSION 1.0
  SOURCES
    ${katalogue_entries_src}
)
