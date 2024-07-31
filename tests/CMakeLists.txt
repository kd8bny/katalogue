# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>
find_package(Qt6 ${QT_MIN_VERSION} REQUIRED Test)

ecm_add_test(databaseItemTest.cpp
    TEST_NAME "databaseItemTest"
    LINK_LIBRARIES Qt6::Test katalogue_data
)

ecm_add_test(databaseAttributeTest.cpp
    TEST_NAME "databaseAttributeTest"
    LINK_LIBRARIES Qt6::Test katalogue_data
)

ecm_add_test(databaseEventTest.cpp
    TEST_NAME "databaseEventTest"
    LINK_LIBRARIES Qt6::Test katalogue_data
)

ecm_add_test(databaseNoteTest.cpp
    TEST_NAME "databaseNoteTest"
    LINK_LIBRARIES Qt6::Test katalogue_data
)
