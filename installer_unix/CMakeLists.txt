cmake_minimum_required(VERSION 3.16)

project(installer_unix VERSION ${PROJECT_VERSION})

set(WIX_INSTALLER_PLATFORM_NAME "WIN")

set(CMAKE_CURRENT_BINARY_DIR ${PROJECT_BINARY_DIR}/output)

# message(FATAL_ERROR ">>> ${PROJECT_NAME}")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Alexey Potapenko")
set(CPACK_PROJECT_NAME "${PROJECT_NAME}")
set(CPACK_CMAKE_GENERATOR "Unix Makefiles")
set(CPACK_GENERATOR "DEB") #"STGZ;TGZ;TZ")
# set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_SOURCE_DIR};game;${CMAKE_SOURCE_DIR}/engine;") #CMake;ALL;/")
# set(CPACK_NSIS_DISPLAY_NAME "CMake 2.5")
set(CPACK_OUTPUT_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/CPackConfig.cmake")
# set(CPACK_PACKAGE_DESCRIPTION_FILE "/home/andy/vtk/CMake/Copyright.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Match3 game")
# set(CPACK_PACKAGE_EXECUTABLES "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/WinMoBubbles")
set(CPACK_PACKAGE_FILE_NAME "${GAME_NAME}")
# set(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake 2.5")
# set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "CMake 2.5.0")
set(CPACK_PACKAGE_NAME "${GAME_NAME}")
set(CPACK_PACKAGE_VENDOR "GrottansBarn")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
# set(CPACK_RESOURCE_FILE_LICENSE "/home/andy/vtk/CMake/Copyright.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/readme.md")
# set(CPACK_RESOURCE_FILE_WELCOME "/home/andy/vtk/CMake/Templates/CPack.GenericWelcome.txt")
# set(CPACK_SOURCE_GENERATOR "TBZ2")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/CPackSourceConfig.cmake")
# set(CPACK_SOURCE_PACKAGE_FILE_NAME "cmake-2.5.0")
set(CPACK_STRIP_FILES "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/WinMoBubbles")
set(CPACK_SOURCE_STRIP_FILES "")
# set(CPACK_SYSTEM_NAME "Linux x86_64")
# set(CPACK_TOPLEVEL_TAG "Linux x86_64")

# set(CPACK_LIBRARY_OUTPUT_PATH ${})
# set(CPACK_EXECUTABLE_OUTPUT_PATH ${})

# List of four values that specify what project to install. 
# The four values are: Build directory, Project Name, Project Component, Directory. 
# If omitted, CPack will build an installer that installs everything.
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_INSTALL_CMAKE_PROJECTS 
    "${CMAKE_SOURCE_DIR}/build; game; winmo_bin; /"
    "${CMAKE_SOURCE_DIR}/build; engine; winmo_engine; /"
    "${CMAKE_SOURCE_DIR}/build; data; winmo_data; /")
# set(CPACK_COMPONENTS_ALL
#         winmo_engine
#         winmo_bin
#         winmo_data
# )

# message(FATAL_ERROR ">>> ${PROJECT_BINARY_DIR}")

add_custom_target(PACKAGING # -G TBZ2
                    COMMAND cpack -G DEB --config "${CPACK_OUTPUT_CONFIG_FILE}" -B "${CMAKE_CURRENT_BINARY_DIR}/" #../output"
                    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/output
                    COMMENT "-- Run CPack for UNIX deb package build"
)

add_custom_target(installer-unix-ALL)

add_dependencies(installer-unix-ALL PACKAGING)

include(CPack)

cpack_add_component(winmo_engine
                    DESCRIPTION "GrottansBarn's engine library"
                    REQUIRED
                    )
cpack_add_component(winmo_bin
                    DESCRIPTION "WinMoBubbles executable"
                    REQUIRED
                    )
cpack_add_component(winmo_data
                    DESCRIPTION "WinMoBubbles data files"
                    REQUIRED
                    )