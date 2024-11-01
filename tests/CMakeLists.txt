# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2022 Daryl Bennett <kd8bny@gmail.com>
find_package(Qt6 ${QT_MIN_VERSION} REQUIRED Test)

ecm_add_test(itemDatabaseTest.cpp
    TEST_NAME "itemDatabaseTest"
    LINK_LIBRARIES Qt6::Test katalogue_data
)

ecm_add_test(attributeDatabaseTest.cpp
    TEST_NAME "attributeDatabaseTest"
    LINK_LIBRARIES Qt6::Test katalogue_data
)

ecm_add_test(eventDatabaseTest.cpp
    TEST_NAME "eventDatabaseTest"
    LINK_LIBRARIES Qt6::Test katalogue_data
)

ecm_add_test(noteDatabaseTest.cpp
    TEST_NAME "noteDatabaseTest"
    LINK_LIBRARIES Qt6::Test katalogue_data
)

ecm_add_test(taskDatabaseTest.cpp
    TEST_NAME "taskDatabaseTest"
    LINK_LIBRARIES Qt6::Test katalogue_data
)

ecm_add_test(documentDatabaseTest.cpp
    TEST_NAME "documentDatabaseTest"
    LINK_LIBRARIES Qt6::Test katalogue_data
)
