set(COMPILER_GET_VERSION_SWITCH "-v")

execute_process(COMMAND "${CMAKE_C_COMPILER}" ${COMPILER_GET_VERSION_SWITCH}
        OUTPUT_VARIABLE COMPILER_VERSION_FULL
        ERROR_VARIABLE COMPILER_VERSION_FULL)
message(STATUS "C Compiler command: ${CMAKE_C_COMPILER}")
message(STATUS "C Compiler version: ${COMPILER_VERSION_FULL}")
message(STATUS "C Compiler id: ${CMAKE_CXX_COMPILER_ID}")

execute_process(COMMAND "${CMAKE_CXX_COMPILER}" ${COMPILER_GET_VERSION_SWITCH}
        OUTPUT_VARIABLE COMPILER_VERSION_FULL
        ERROR_VARIABLE COMPILER_VERSION_FULL)
message(STATUS "CXX Compiler command: ${CMAKE_CXX_COMPILER}")
message(STATUS "CXX Compiler version: ${COMPILER_VERSION_FULL}")
message(STATUS "CXX Compiler id: ${CMAKE_CXX_COMPILER_ID}")


string(TOLOWER "${COMPILER_VERSION_FULL}" COMPILER_VERSION_FULL_LOWER)

# clang on Linux and Mac OS X before 10.9
if("${COMPILER_VERSION_FULL}" MATCHES ".*clang version.*")
    set(COMPILER_FAMILY "clang")
    string(REGEX REPLACE ".*clang version ([0-9]+\\.[0-9]+).*" "\\1"
            COMPILER_VERSION "${COMPILER_VERSION_FULL}")

    # LLVM 3.6 on Mac OS X 10.9 and later
elseif("${COMPILER_VERSION_FULL}" MATCHES ".*based on LLVM 3\\.6\\..*")
    set(COMPILER_FAMILY "clang")
    set(COMPILER_VERSION "3.6.0svn")

    # clang on Mac OS X 10.9 and later
elseif("${COMPILER_VERSION_FULL}" MATCHES ".*based on LLVM.*")
    set(COMPILER_FAMILY "clang")
    string(REGEX REPLACE ".*based on LLVM ([0-9]+\\.[0.9]+).*" "\\1"
            COMPILER_VERSION "${COMPILER_VERSION_FULL}")

    # clang on Mac OS X, XCode 7.
elseif("${COMPILER_VERSION_FULL}" MATCHES ".*clang-7")
    set(COMPILER_FAMILY "clang")
    set(COMPILER_VERSION "3.7.0svn")

    # clang on Mac OS X, XCode 8.
elseif("${COMPILER_VERSION_FULL}" MATCHES ".*clang-802")
    set(COMPILER_FAMILY "clang")
    set(COMPILER_VERSION "3.9.0svn")

    # clang on Mac OS X, XCode 8.
elseif("${COMPILER_VERSION_FULL}" MATCHES ".*clang-8")
    set(COMPILER_FAMILY "clang")
    set(COMPILER_VERSION "3.8.0svn")

    # clang on Mac OS X, XCode 9.
elseif("${COMPILER_VERSION_FULL}" MATCHES ".*clang-9")
    set(COMPILER_FAMILY "clang")
    set(COMPILER_VERSION "4.0.0svn")

    # clang on Mac OS X, XCode 10.
elseif("${COMPILER_VERSION_FULL}" MATCHES ".*clang-10")
    set(COMPILER_FAMILY "clang")
    set(COMPILER_VERSION "4.1.0svn")

    # gcc
elseif("${COMPILER_VERSION_FULL_LOWER}" MATCHES ".*gcc[ -]version.*")
    set(COMPILER_FAMILY "gcc")
    string(REGEX REPLACE ".*gcc[ -]version ([0-9\\.]+).*" "\\1"
            COMPILER_VERSION "${COMPILER_VERSION_FULL_LOWER}")
else()
    message(FATAL_ERROR "Unknown compiler. Version info:\n${COMPILER_VERSION_FULL}")
endif()
message(STATUS "Selected compiler ${COMPILER_FAMILY} ${COMPILER_VERSION}")
