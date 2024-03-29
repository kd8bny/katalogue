# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

add_executable(katalogue
    main.cpp
    about.cpp
    katalogue.cpp
    data/database.cpp
    data/attribute.cpp
    data/event.cpp
    data/item.cpp
    data/note.cpp
    data/task.cpp
    models/attributeModel.cpp
    models/attributeCategoryModel.cpp
    models/eventModel.cpp
    models/eventCategoryModel.cpp
    models/itemModel.cpp
    models/itemComponentModel.cpp
    models/itemTypeModel.cpp
    models/itemParentModel.cpp
    models/noteModel.cpp
    resources.qrc)

target_link_libraries(katalogue
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickControls2
    Qt5::Svg
    Qt5::Sql
    KF5::I18n
    KF5::CoreAddons
    KF5::ConfigCore
    KF5::ConfigGui)

kconfig_add_kcfg_files(katalogue GENERATE_MOC katalogueconfig.kcfgc)
target_compile_options(katalogue PUBLIC)
install(TARGETS katalogue ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
