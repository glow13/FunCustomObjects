cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(FunCustomObjects VERSION 1.0.0)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

if (DEFINED ENV{CI})
    message(STATUS "Downloading CustomObjectsAPI from '${ARTIFACT_DOWNLOAD_URL}'")

    set(DIRECTORY "${CMAKE_BINARY_DIR}/geode-deps/glow12.custom-objects-api")

    set(BUILD_OUTPUT_FILE "${DIRECTORY}/Build Output.zip")
    set(MOD_GEODE_FILE "${DIRECTORY}/glow12.custom-objects-api.geode")

    file(MAKE_DIRECTORY "${DIRECTORY}")
    file(DOWNLOAD "${ARTIFACT_DOWNLOAD_URL}" "${BUILD_OUTPUT_FILE}")

    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf "${BUILD_OUTPUT_FILE}" WORKING_DIRECTORY "${DIRECTORY}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf "${MOD_GEODE_FILE}" WORKING_DIRECTORY "${DIRECTORY}")

    file(WRITE "${DIRECTORY}/geode-dep-options.json" "{ \"required\": true }")

    file(REMOVE "${BUILD_OUTPUT_FILE}")
    file(REMOVE "${MOD_GEODE_FILE}")

    message(STATUS "Finished downloading and extracting CustomObjectsAPI!")

    setup_geode_mod(${PROJECT_NAME} EXTERNALS glow12.custom-objects-api:1.0.0-beta.1)
else()
    setup_geode_mod(${PROJECT_NAME})
endif()