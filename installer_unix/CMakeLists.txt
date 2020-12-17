cmake_minimum_required(VERSION 3.16)

project(installer_unix VERSION ${PROJECT_VERSION})

set(WIX_INSTALLER_PLATFORM_NAME "WIN")

set(CMAKE_CURRENT_BINARY_DIR ${PROJECT_BINARY_DIR}/output)

add_custom_target(installer-unix-ALL)

add_custom_target(RUN_CPACK_UNIX 
    COMMAND "cpack" -G DEB
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "-- Run CPack for UNIX deb package build"
)

add_dependencies(installer-unix-ALL RUN_CPACK_UNIX)

include(CPack)
# cpack -G DEB