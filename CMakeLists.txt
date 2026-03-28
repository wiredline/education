cmake_minimum_required(VERSION 3.16)
project(EducationProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(spdlog REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(fmt REQUIRED)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(EXEC_NAME main_release)
else()
    set(EXEC_NAME main_debug)
endif()

add_executable(${EXEC_NAME} ${SOURCES})

target_include_directories(${EXEC_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/include
)

target_link_libraries(${EXEC_NAME}
    PRIVATE
    spdlog::spdlog
    fmt::fmt
    nlohmann_json::nlohmann_json
)

if(MSVC)
    target_compile_options(${EXEC_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${EXEC_NAME} PRIVATE -Wall -Wextra -Werror)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${EXEC_NAME} PRIVATE DEBUG)
    message(STATUS "Build type: Debug")
else()
    target_compile_definitions(${EXEC_NAME} PRIVATE NDEBUG)
    message(STATUS "Build type: Release")
endif()