cmake_minimum_required(VERSION 3.10)

project(installer VERSION ${PROJECT_VERSION})

##### turn off cmake extentions
if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

