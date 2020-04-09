file(GLOB SOURCES *.cpp)

set(LIB_NAME jvm)
if (BUILD_JVM_STATIC)
    message(STATUS "Build static library")
    add_library(${LIB_NAME} STATIC ${SOURCES})
else ()
    message(STATUS "Build shared library")
    add_library(${LIB_NAME} SHARED ${SOURCES})
endif ()

# set lib output name
set_target_properties(${LIB_NAME} PROPERTIES OUTPUT_NAME "${LIB_NAME}")

# link
target_link_libraries(jvm PRIVATE spdlog::spdlog)