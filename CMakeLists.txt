cmake_minimum_required(VERSION 3.16)

option(BUILD_DOC_OPTION "Build Docs" OFF)
option(BUILD_INSTALLER_OPTION "Build installer" OFF)
option(ANDROID_PREPARE_OPTION "Configure SDL android-project" OFF)

if(NOT UNIX AND NOT WIN32 AND NOT ANDROID)
    message(FATAL_ERROR "Unsupported operation system detected")
endif()

set(GAME_NAME WinMoBubbles)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# set version
include(${CMAKE_CURRENT_LIST_DIR}/version/version.cmake)

project(${GAME_NAME} 
            DESCRIPTION "Match-3 game from Windows Mobile"
            VERSION ${PROJECT_VERSION} 
            LANGUAGES CXX)

set(GAME_INSTALL_PATH "/home/games/${GAME_NAME}") #${CMAKE_INSTALL_BINDIR}/${GAME_NAME}  # GNU /bin folder

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/libs")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/libs")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/libs")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}" ${CMAKE_MODULE_PATH})

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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

# turn off cmake extentions
if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

set(EXECUTABLE_NAME "WinMoBubbles")
set(ENGINE_LIB_NAME engine)

add_custom_target(whole_project)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/SDL2-2.0.16)
include_directories(${CMAKE_CURRENT_LIST_DIR}/thirdparty/SDL2-2.0.16/include)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/utils)
include_directories(${CMAKE_CURRENT_LIST_DIR}/utils/include)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/engine)

add_subdirectory(${CMAKE_SOURCE_DIR}/data)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/game)

# Docs build as an option and set it to ON by default
if(BUILD_DOC_OPTION AND NOT ANDROID)
    message(STATUS "Docs build started")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/docs)
endif()

# installer build as an option and set it to ON by default
if(BUILD_INSTALLER_OPTION AND NOT ANDROID)
    message(STATUS "Installer build started")
    if(WIN32)
        add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/installer_win)
    endif()
    if(UNIX)
        add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/installer_unix)
    endif()
endif()
