# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

set(katalogue_adapters_src
    systemClipboardAdapter.h
)

add_library(katalogue_adapters STATIC
    ${katalogue_adapters_src}
)

target_link_libraries(katalogue_adapters
    Qt6::Core
    Qt6::Qml
    Qt6::Gui
)

qt_add_qml_module(adapters
  URI com.kd8bny.katalogue.adapters
  VERSION 1.0
)
