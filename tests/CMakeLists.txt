# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>
find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS Test)


add_executable(katalogueTests
    databaseItemTest.cpp
    # databaseAttributeTest.cpp

    ../src/constants/defaults.cpp
    ../src/data/database.cpp
    ../src/data/attribute.cpp
    ../src/data/event.cpp
    ../src/data/item.cpp
    ../src/data/note.cpp
    ../src/data/task.cpp
)

target_link_libraries(katalogueTests PRIVATE Qt::Test
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Svg
    Qt6::Sql
    Qt6::Widgets
    Qt6::Test
    KF6::I18n
    KF6::CoreAddons
    KF6::ConfigCore
    KF6::ConfigGui
)

add_test(NAME databaseItemTest COMMAND katalogueTests)
