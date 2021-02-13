cmake_minimum_required(VERSION 3.5)

file(READ "version.txt" ver)

string(REGEX MATCH "VERSION_MAJOR ([0-9]*)" _ ${ver})
set(ver_major ${CMAKE_MATCH_1})

string(REGEX MATCH "VERSION_MINOR ([0-9]*)" _ ${ver})
set(ver_minor ${CMAKE_MATCH_1})

string(REGEX MATCH "VERSION_PATCH ([0-9]*)" _ ${ver})
set(ver_patch ${CMAKE_MATCH_1})

message(STATUS "Version: ${ver_major}.${ver_minor}.${ver_patch}")

MATH(EXPR ver_patch "${ver_patch}+1")

set(OUT_VERSION_FILE "VERSION_MAJOR ${ver_major}\nVERSION_MINOR ${ver_minor}\nVERSION_PATCH ${ver_patch}")

file(WRITE "version.txt" ${OUT_VERSION_FILE})