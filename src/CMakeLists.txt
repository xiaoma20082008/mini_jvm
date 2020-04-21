#add_definitions(-D PRINT_DEBUG)

set(SRC_LIST
        classfile/AccessFlags.cpp
        classfile/ClassFile.cpp
        classfile/ClassReader.cpp
        classfile/ClassLoader.cpp

        interpreter/Interpreter.cpp

        memory/Allocation.cpp

        runtime/ConstantPool.cpp
        runtime/JvmType.cpp
        runtime/JvmClass.cpp
        runtime/JvmThread.cpp

        MiniVM.cpp
        )

set(LIB_NAME jvm)

if (BUILD_JVM_STATIC)
    message(STATUS "Build static library")
    add_library(${LIB_NAME} STATIC ${SRC_LIST})
else ()
    message(STATUS "Build shared library")
    add_library(${LIB_NAME} SHARED ${SRC_LIST})
endif ()

set_target_properties(${LIB_NAME} PROPERTIES OUTPUT_NAME "${LIB_NAME}")

target_link_libraries(${LIB_NAME} PRIVATE spdlog::spdlog)