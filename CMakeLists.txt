# SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.21.1)

project(katalogue VERSION 0.1)

include(FeatureSummary)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_MIN_VERSION "6.6.0")
set(KF6_MIN_VERSION "6.0.0")

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMPoQmTools)

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX KATALOGUE
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/version-katalogue.h"
)

find_package(Qt6 ${QT6_MIN_VERSION} REQUIRED COMPONENTS Core Gui Qml QuickControls2 Svg Sql Quick Widgets)
find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS Kirigami CoreAddons Config I18n)
# find_package(Catch2 3 REQUIRED)

add_subdirectory(src)

install(PROGRAMS org.kde.katalogue.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.katalogue.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

# # These tests can use the Catch2-provided main
# add_executable(tests tests/databaseTest.cpp)
# target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
