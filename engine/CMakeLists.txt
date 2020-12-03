cmake_minimum_required(VERSION 3.17)

project(engine)

set(ENGINE_LIB_NAME engine)

file(GLOB_RECURSE ENGINE_SRC_FILES
    "${CMAKE_CURRENT_LIST_DIR}/*.hxx"
    "${CMAKE_CURRENT_LIST_DIR}/*.cxx"
    )

add_library(${ENGINE_LIB_NAME} 
            SHARED
            ${ENGINE_SRC_FILES})

if(WIN32)
    # set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    target_compile_definitions(${ENGINE_LIB_NAME} PRIVATE "-DOM_DECLSPEC=__declspec(dllexport)")
endif(WIN32)

target_compile_features(${ENGINE_LIB_NAME} PUBLIC cxx_std_17)

if (UNIX)
    target_link_libraries(${ENGINE_LIB_NAME}
        SDL2
        -lGL
        )
elseif (WIN32)
    target_link_libraries(${ENGINE_LIB_NAME}
        PRIVATE SDL2 
                SDL2main
                opengl32
        )
endif()

# set_property(TARGET ${ENGINE_LIB_NAME} APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR})

# target and dependencies
# add_custom_target(${ENGINE_LIB_NAME})
# add_dependencies(build-ALL ${ENGINE_LIB_NAME})