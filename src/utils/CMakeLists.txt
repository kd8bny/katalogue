# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

set(katalogue_utils_src
    documentIOHelper.cpp
)

add_library(katalogue_utils STATIC
    ${katalogue_utils_src}
)

target_link_libraries(katalogue_utils
    katalogue_entries
    katalogue_data
    Qt6::Core
)
