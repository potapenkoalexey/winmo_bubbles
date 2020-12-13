cmake_minimum_required(VERSION 3.16)

if(NOT UNIX AND NOT WIN32 AND NOT ANDROID)
    message(FATAL_ERROR "Unsupported operation system detected")
endif()

set(GAME_NAME WinMoBubbles)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(PROJECT_VERSION_MAJOR 2)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

project(${GAME_NAME} 
            DESCRIPTION "Match-3 game from Windows Mobile"
            VERSION ${PROJECT_VERSION} 
            LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin")

# configure build
set(CMAKE_SUPPRESS_REGENERATION true)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
add_definitions( -D_CRT_SECURE_NO_WARNINGS )

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
  message(STATUS "Added parallel build arguments to CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
  message(STATUS "Added parallel build arguments to CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
endif()

# turn off cmake extentions
if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/libs/SDL2-2.0.8)
include_directories(${CMAKE_CURRENT_LIST_DIR}/libs/SDL2-2.0.8/include)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/engine)
include_directories(${CMAKE_CURRENT_LIST_DIR}/engine)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/game)
include_directories(${CMAKE_CURRENT_LIST_DIR}/game)

if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
    # Docs build as an option and set it to ON by default
    option(BUILD_DOC "Build Docs" ON)
    message(STATUS "Docs build started")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/docs)

    if(WIN32)
        # installer build as an option and set it to ON by default
        option(BUILD_INSTALLER "Build installer" ON)
        message(STATUS "Installer build started")
        add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/installer)
    endif()
endif()
