cmake_minimum_required(VERSION 3.5)
project(MINI_JVM VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/jvm")


add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(third_party)

add_executable(mini_jvm main.cpp)

target_include_directories(jvm PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/jvm")

target_link_libraries(mini_jvm jvm)
