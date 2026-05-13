cmake_minimum_required(VERSION 3.16)
project(EducationProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(spdlog REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(fmt REQUIRED)
find_package(PostgreSQL REQUIRED)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

include(FetchContent)
FetchContent_Declare(
    mathfunc
    GIT_REPOSITORY https://github.com/wiredline/mathfunc.git
    GIT_TAG main
)
FetchContent_MakeAvailable(mathfunc)

file(GLOB MATHFUNC_SOURCES "${mathfunc_SOURCE_DIR}/*.cpp")

add_executable(main ${SOURCES} ${MATHFUNC_SOURCES})

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(main PROPERTIES OUTPUT_NAME main_release)
else()
    set_target_properties(main PROPERTIES OUTPUT_NAME main_debug)
endif()

target_include_directories(main
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/include
    ${mathfunc_SOURCE_DIR}
)

target_link_libraries(main
    PRIVATE
    spdlog::spdlog
    fmt::fmt
    nlohmann_json::nlohmann_json
    PostgreSQL::PostgreSQL
)

if(MSVC)
    target_compile_options(main PRIVATE /W4 /WX)
else()
    target_compile_options(main PRIVATE -Wall -Wextra -Werror)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(main PRIVATE DEBUG)
    message(STATUS "Build type: Debug")
else()
    target_compile_definitions(main PRIVATE NDEBUG)
    message(STATUS "Build type: Release")
endif()

# --- clang-format
find_program(CLANG_FORMAT_EXE NAMES clang-format)

if(CLANG_FORMAT_EXE)

    file(GLOB_RECURSE ALL_SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/include/*.h"
        "${PROJECT_SOURCE_DIR}/src/include/*.hpp"
    )

    add_custom_target(
        clangformat
        COMMAND ${CLANG_FORMAT_EXE}
        -i
        ${ALL_SOURCE_FILES}
        COMMENT "Running clang-format..."
    )
endif()
# --- clang-tidy
find_program(CLANG_TIDY_EXE NAMES clang-tidy)

if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE})
endif()