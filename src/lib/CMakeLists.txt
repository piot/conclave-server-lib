cmake_minimum_required(VERSION 3.17)
project(conclave_server_lib C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-std=c99")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(isDebug TRUE)
else ()
    message("no debug")
    set(isDebug FALSE)
endif ()

if (isDebug)
    message("Debug build detected")
    add_compile_definitions(CONFIGURATION_DEBUG TYRAN_CONFIGURATION_DEBUG)
    set(CONFIGURATION_DEBUG 1)
endif ()

set(DEPS ../../deps/)

file(GLOB lib_src FOLLOW_SYMLINKS "*.c")

add_library(conclave_server_lib STATIC ${lib_src})

target_compile_options(
        conclave_server_lib
        PRIVATE -Wall
        -Wextra
        -Wshadow
        -Wstrict-aliasing
        -ansi
        -pedantic
        -Wno-declaration-after-statement
        -Wno-extra-semi-stmt
        -Wno-undef
        -Wno-unused-variable
        -Wno-unused-parameter
        -Wno-padded
        -Werror=implicit-function-declaration
        -Werror=incompatible-pointer-types
        -Werror=missing-prototypes
        -Werror=int-conversion
        -Werror=return-type
        -Werror=incompatible-function-pointer-types)

if (COMPILER_CLANG)
    target_compile_options(
            conclave_server_lib PRIVATE -Wmost -Weverything
            -Werror=missing-variable-declarations)
endif ()

target_include_directories(conclave_server_lib
        PUBLIC ${DEPS}piot/flood-c/src/include)
target_include_directories(conclave_server_lib
        PUBLIC ${DEPS}piot/conclave-serialize-c/src/include)
target_include_directories(conclave_server_lib
        PUBLIC ${DEPS}piot/clog/src/include)
target_include_directories(conclave_server_lib
        PUBLIC ${DEPS}piot/tiny-libc/src/include)
target_include_directories(conclave_server_lib
        PUBLIC ${DEPS}piot/imprint/src/include)
target_include_directories(conclave_server_lib
        PUBLIC ${DEPS}piot/secure-random-c/src/include)

target_include_directories(conclave_server_lib PUBLIC ../include)
