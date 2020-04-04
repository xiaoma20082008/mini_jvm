file(GLOB SOURCES *.cpp)

if (BUILD_JVM_STATIC)
    message(STATUS "Build static library")
    add_library(jvm STATIC ${SOURCES})
else ()
    message(STATUS "Build shared library")
    add_library(jvm SHARED ${SOURCES})
endif ()

target_link_libraries(jvm PRIVATE spdlog::spdlog)