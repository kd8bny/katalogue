# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

add_executable(tests
    main.cpp
    databaseTest.cpp

    ../src/constants/defaults.cpp
    ../src/data/database.cpp
    ../src/data/attribute.cpp
    ../src/data/event.cpp
    ../src/data/item.cpp
    ../src/data/note.cpp
    ../src/data/task.cpp
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Svg
    Qt6::Sql
    Qt6::Widgets
    KF6::I18n
    KF6::CoreAddons
    KF6::ConfigCore
    KF6::ConfigGui
)
