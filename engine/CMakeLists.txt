cmake_minimum_required(VERSION 3.16)

project(engine)

set(ENGINE_LIB_NAME engine)

file(GLOB_RECURSE ENGINE_SRC_FILES
    "${CMAKE_SOURCE_DIR}/game/global_variables.hxx"
    "${CMAKE_SOURCE_DIR}/game/global_variables.cxx"
    "${CMAKE_CURRENT_LIST_DIR}/*.hxx"
    "${CMAKE_CURRENT_LIST_DIR}/*.cxx"
    )

add_library(${PROJECT_NAME} 
            STATIC #SHARED
            ${ENGINE_SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/libs/SDL2-2.0.8/include/)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src/)

if(WIN32)
    # set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    target_compile_definitions(${PROJECT_NAME} PRIVATE "-DOM_DECLSPEC=__declspec(dllexport)")
endif(WIN32)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if (UNIX)
    target_link_libraries(${PROJECT_NAME}
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

# for cpack
install(TARGETS ${PROJECT_NAME}
        OBJECTS #LIBRARY
        #DESTINATION / #./${GAME_INSTALL_PATH}
        COMPONENT winmo_engine
)

# set_property(TARGET ${ENGINE_LIB_NAME} APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR})