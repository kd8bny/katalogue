# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

set(katalogue_data_src
    databaseInit.cpp
    databaseUtils.cpp

    itemDatabase.cpp
    attributeDatabase.cpp
    eventDatabase.cpp
    noteDatabase.cpp
    taskDatabase.cpp
    documentDatabase.cpp
)

add_library(katalogue_data STATIC
    ${katalogue_data_src}
)

target_link_libraries(katalogue_data
    katalogue_entries
    Qt6::Core
    Qt6::Sql
)
