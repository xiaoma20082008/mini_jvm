file(GLOB SOURCES *.cpp)

if (BUILD_JVM_STATIC)
    add_library(jvm STATIC ${SOURCES})
else ()
    add_library(jvm SHARED ${SOURCES})
endif ()

target_link_libraries(jvm PRIVATE spdlog::spdlog)