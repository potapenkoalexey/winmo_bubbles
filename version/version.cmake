cmake_minimum_required(VERSION 3.16)

if(CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug")

    set_property(GLOBAL PROPERTY PROJECT_VERSION_MAJOR 2)
    set_property(GLOBAL PROPERTY PROJECT_VERSION_MINOR 99)
    set_property(GLOBAL PROPERTY PROJECT_VERSION_PATCH 99)

else()

    file(READ "${CMAKE_CURRENT_LIST_DIR}/version/version.txt" ver)

    string(REGEX MATCH "VERSION_MAJOR ([0-9]*)" _ ${ver})
    set(ver_major ${CMAKE_MATCH_1})
    string(REGEX MATCH "VERSION_MINOR ([0-9]*)" _ ${ver})
    set(ver_minor ${CMAKE_MATCH_1})
    string(REGEX MATCH "VERSION_PATCH ([0-9]*)" _ ${ver})
    set(ver_patch ${CMAKE_MATCH_1})

    MATH(EXPR ver_patch "${ver_patch}+1")

    set(OUT_VERSION_FILE "VERSION_MAJOR ${ver_major}\nVERSION_MINOR ${ver_minor}\nVERSION_PATCH ${ver_patch}")

    file(WRITE "${CMAKE_CURRENT_LIST_DIR}/version/version.txt" ${OUT_VERSION_FILE})

    set_property(GLOBAL PROPERTY PROJECT_VERSION_MAJOR ${ver_major})
    set_property(GLOBAL PROPERTY PROJECT_VERSION_MINOR ${ver_minor})
    set_property(GLOBAL PROPERTY PROJECT_VERSION_PATCH ${ver_patch})

endif()

get_property(major GLOBAL PROPERTY PROJECT_VERSION_MAJOR)
get_property(minor GLOBAL PROPERTY PROJECT_VERSION_MINOR)
get_property(patch GLOBAL PROPERTY PROJECT_VERSION_PATCH)

set(PROJECT_VERSION ${major}.${minor}.${patch})

message(STATUS "Version: ${PROJECT_VERSION}")