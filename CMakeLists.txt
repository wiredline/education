cmake_minimum_required(VERSION 3.16)
project(EducationProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(spdlog REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(fmt REQUIRED)

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