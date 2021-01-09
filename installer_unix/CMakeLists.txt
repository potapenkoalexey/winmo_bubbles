cmake_minimum_required(VERSION 3.16)

project(installer_unix VERSION ${PROJECT_VERSION})

set(WIX_INSTALLER_PLATFORM_NAME "WIN")

set(CMAKE_CURRENT_BINARY_DIR ${PROJECT_BINARY_DIR}/output)

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Alexey Potapenko")
set(CPACK_PROJECT_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_FILE_NAME "${EXECUTABLE_NAME}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Match3 game")
set(CPACK_PACKAGE_NAME "${EXECUTABLE_NAME}")
set(CPACK_PACKAGE_VENDOR "GrottansBarn")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/readme.md")
set(CPACK_CMAKE_GENERATOR "Unix Makefiles")
set(CPACK_OUTPUT_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/CPackConfig.cmake")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/CPackSourceConfig.cmake")
# set(CPACK_PACKAGE_EXECUTABLES "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/WinMoBubbles")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "/home/WinMoBubbles")
# set(CPACK_PACKAGE_DESCRIPTION_FILE "/home/andy/vtk/CMake/Copyright.txt")
# set(CPACK_RESOURCE_FILE_LICENSE "/home/andy/vtk/CMake/Copyright.txt")

set(CPACK_ARCHIVE_COMPONENT_INSTALL OFF)

# List of four values that specify what project to install. 
# The four values are: Build directory, Project Name, Project Component, Directory. 
# If omitted, CPack will build an installer that installs everything.
set(CPACK_INSTALL_CMAKE_PROJECTS 
    "${CMAKE_SOURCE_DIR}/build/game;game;ALL;/"
    "${CMAKE_SOURCE_DIR}/build/data;data;ALL;/"
)

set(CPACK_COMPONENTS_ALL
        winmo_bin
        winmo_data
)

include(CPack)

# hotfix SDL2 build problem
add_custom_target(LINK_TARGET
                  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libSDL2-2.0.so.0.8.0 ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/SDL2-2.0.8/libSDL2.so)

# unused by task build
add_custom_target(PACKAGING # -G TBZ2
                    COMMAND cpack -G ZIP # --config "${CPACK_OUTPUT_CONFIG_FILE}" -B "${CMAKE_CURRENT_BINARY_DIR}/" --verbose
                    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/output
                    COMMENT "-- Run CPack for UNIX package build"
)

add_custom_target(installer-unix-ALL)

add_dependencies(PACKAGING LINK_TARGET)
add_dependencies(installer-unix-ALL PACKAGING)