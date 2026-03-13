cmake_minimum_required(VERSION 3.14)
project(calcproject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    mathfunc
    GIT_REPOSITORY https://github.com/wiredline/mathfunc.git
    GIT_TAG main
)
FetchContent_MakeAvailable(mathfunc)

add_executable(main main.cpp)

target_include_directories(main PRIVATE ${mathfunc_SOURCE_DIR})
find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
    set_target_properties(main PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()

find_program(CLANG_FORMAT_EXE NAMES clang-format)
if(CLANG_FORMAT_EXE)
    message(STATUS "clang-format found: ${CLANG_FORMAT_EXE}")
    file(GLOB_RECURSE ALL_CXX_FILES
         "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    )
    add_custom_target(
        clang_format
        COMMAND ${CLANG_FORMAT_EXE} -i ${ALL_CXX_FILES}
        COMMENT "Formatting all source files with clang-format"
    )
endif()