cmake_minimum_required(VERSION 3.5)
project(mini_jvm
        LANGUAGES C CXX
        VERSION 1.0.0)

message(STATUS "Project: ${PROJECT_NAME} version: ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

# cmake插件目录
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake_plugins")

# Debug模式 选项: Release Debug MniSizeRel RelWithDebInfo
set(CMAKE_BUILD_TYPE "Debug")

# c/c++
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译参数
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -std=c++17 -o0 -Werror -g2 -glldb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -std=c++17 -o3 -Werror")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

#
include(CompilerInfo)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/jvm")

# 是否构建静态库
option(BUILD_JVM_STATIC "Build the libjvm static libraries" ON)

option(DEBUG "Print debug messages" ON)

if (WIN32)
    message(STATUS "This is Windows.")
elseif (APPLE)
    message(STATUS "This is MacOS.")
elseif (UNIX)
    message(STATUS "This is Linux.")
endif ()

enable_testing()

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(third_party)

add_executable(mini_java mini_java.cpp)

target_include_directories(mini_java PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/jvm")

target_link_libraries(mini_java jvm)
