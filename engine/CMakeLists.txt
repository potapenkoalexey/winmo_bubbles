cmake_minimum_required(VERSION 3.16)

project(${ENGINE_LIB_NAME})

file(GLOB_RECURSE ENGINE_SRC_FILES
    "${CMAKE_SOURCE_DIR}/game/global_variables.hxx"
    "${CMAKE_SOURCE_DIR}/game/global_variables.cxx"
    "${CMAKE_CURRENT_LIST_DIR}/*.hxx"
    "${CMAKE_CURRENT_LIST_DIR}/*.cxx"
    )

add_library(${PROJECT_NAME} 
            STATIC
            ${ENGINE_SRC_FILES})

find_package(SDL2 REQUIRED)

# set(utils_DIR ${CMAKE_BINARY_DIR})
# find_package(utils REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE "-DOM_DECLSPEC=__declspec(dllexport)")
endif(WIN32)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if (UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        utils
        SDL2-static
        -lGL
        )
elseif (WIN32)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE SDL2 
                SDL2main
                opengl32
        )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${ENGINE_LIB_NAME})

# for cpack
install(TARGETS ${PROJECT_NAME}
        OBJECTS
        DESTINATION .
        COMPONENT winmo_engine
)