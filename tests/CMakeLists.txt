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

# ecm_add_test(databaseEventTest.cpp
#     TEST_NAME "databaseEventTest"
#     LINK_LIBRARIES Qt6::Test katalogue_data
# )

# ecm_add_test(databaseNoteTest.cpp
#     TEST_NAME "databaseNoteTest"
#     LINK_LIBRARIES Qt6::Test katalogue_data
# )

# ecm_add_test(databaseTaskTest.cpp
#     TEST_NAME "databaseTaskTest"
#     LINK_LIBRARIES Qt6::Test katalogue_data
# )
