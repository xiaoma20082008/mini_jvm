set(PATH_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}")

configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/TestConfig.h.in"
        "${CMAKE_CURRENT_SOURCE_DIR}/TestConfig.h"
)

add_executable(class_file_test ClassFileTest.cpp)

target_link_libraries(class_file_test PRIVATE gtest)
target_link_libraries(class_file_test PRIVATE jvm)