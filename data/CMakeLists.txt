cmake_minimum_required(VERSION 3.16)

project(data)

# for cpack
install(DIRECTORY ${CMAKE_SOURCE_DIR}/data
        DESTINATION .
        COMPONENT winmo_data
        PATTERN CMakeLists.txt EXCLUDE 
)
