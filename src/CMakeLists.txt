# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>

add_subdirectory(entries)
add_subdirectory(data)

set(katalogue_models_src
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

add_library(katalogue_models STATIC
    ${katalogue_models_src}
)

target_link_libraries(katalogue_models
    katalogue_data
    Qt6::Core
    Qt6::Sql
    Qt6::Qml
)

add_executable(katalogue
    main.cpp
    about.cpp
    katalogue.cpp
    resources.qrc

    constants/defaults.cpp
)

target_link_libraries(katalogue
    katalogue_data
    katalogue_models
    katalogue_entries

    Qt6::Gui
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Widgets
    Qt6::Svg
    Qt6::Sql
    KF6::I18n
    KF6::CoreAddons
    KF6::ConfigCore
    KF6::ConfigGui
)

kconfig_add_kcfg_files(katalogue GENERATE_MOC katalogueconfig.kcfgc)
target_compile_options(katalogue PUBLIC)
install(TARGETS katalogue ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
