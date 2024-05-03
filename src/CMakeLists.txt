# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

add_executable(katalogue
    main.cpp
    about.cpp
    katalogue.cpp
    resources.qrc

    constants/defaults.cpp

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
    models/taskModel.cpp
    )

target_link_libraries(katalogue
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
    KF6::ConfigGui)

kconfig_add_kcfg_files(katalogue GENERATE_MOC katalogueconfig.kcfgc)
target_compile_options(katalogue PUBLIC)
install(TARGETS katalogue ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
