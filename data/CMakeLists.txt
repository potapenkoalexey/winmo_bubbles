cmake_minimum_required(VERSION 3.16)

project(data)

# file(GLOB_RECURSE data_files ${PROJECT_SOURCE_DIR}/*)

# file(COPY ${data_files} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data)

# for cpack
install(DIRECTORY ${CMAKE_SOURCE_DIR}/data
        DESTINATION .
        COMPONENT winmo_data
        PATTERN CMakeLists.txt EXCLUDE 
)