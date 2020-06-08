cmake_minimum_required(VERSION 3.10)

project(winmo_bubbles VERSION 2.1 LANGUAGES CXX)

#if (WIN32)
#SET(CMAKE_C_COMPILER C:\\MinGW\\bin\\)
#SET(CMAKE_CXX_COMPILER C:\\MinGW\\bin\\)
#endif()

if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

file(GLOB_RECURSE GAME_SRC_FILES
            "${CMAKE_CURRENT_LIST_DIR}/src/game/*.hxx"
            "${CMAKE_CURRENT_LIST_DIR}/src/game/*.cxx"
            )
file(GLOB_RECURSE ENGINE_SRC_FILES
            "${CMAKE_CURRENT_LIST_DIR}/src/engine/*.hxx"
            "${CMAKE_CURRENT_LIST_DIR}/src/engine/*.cxx"
            )

add_executable(${PROJECT_NAME}
        ${GAME_SRC_FILES}
        ${CMAKE_CURRENT_LIST_DIR}/src/game/main.cxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/components/block.hxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/components/block.cxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/components/field.hxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/components/field.cxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/components/counter.hxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/components/counter.cxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/components/progress_desk.hxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/components/progress_desk.cxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/components/number.hxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/components/number.cxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/states/select_mode_state.hxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/states/select_mode_state.cxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/states/classic_state.hxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/states/classic_state.cxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/states/extreme_state.hxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/states/extreme_state.cxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/states/level_complete_state.hxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/states/level_complete_state.cxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/states/game_over_state.hxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/states/game_over_state.cxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/global_variables.hxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/global_variables.cxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/settings.hxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/game/settings.cxx
        )

#check this instruction work
#target_link_options(${PROJECT_NAME} PRIVATE -static)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/libs/SDL2-2.0.8)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/src")

set(ENGINE_LIB_NAME engine)
add_library(${ENGINE_LIB_NAME}
        ${ENGINE_SRC_FILES}
#        ${CMAKE_CURRENT_LIST_DIR}/src/engine/engine.hxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/engine/engine.cxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/engine/math_structures.hxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/engine/math_structures.cxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/engine/membuf.hxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/engine/color.hxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/engine/color.cxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/engine/sound_buffer.hxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/engine/sound_buffer.cxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/engine/random_generator.hxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/engine/random_generator.cxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/engine/game_state.hxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/engine/texture.hxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/engine/texture.cxx
#        ${CMAKE_CURRENT_LIST_DIR}/src/engine/vertex_buffer.hxx
        )
target_compile_features(${ENGINE_LIB_NAME} PUBLIC cxx_std_17)

if (UNIX)
target_link_libraries(${ENGINE_LIB_NAME}
    SDL2
    -lGL
    )
endif()
if (WIN32)
target_link_libraries(${ENGINE_LIB_NAME}
    SDL2
    -lopengl32
    )
endif()



if (UNIX)
target_link_libraries(${PROJECT_NAME}
    ${ENGINE_LIB_NAME}
    )
endif()
if (WIN32)
#add_library(sdl STATIC IMPORTED)
#set_property(TARGET sdl PROPERTY
#        IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/libs/win/SDL2.dll)
target_link_libraries(${PROJECT_NAME}
    ${ENGINE_LIB_NAME}
    )
endif()

#set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

#if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
#    # documentation build as an option and set it to ON by default
#    option(BUILD_DOC "Build documentation" ON)
#    find_package(Doxygen)
#    if (DOXYGEN_FOUND)

#        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
#        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
#        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

#        message("Doxygen build started")

#        # note the option ALL which allows to build the docs together with the application
#        add_custom_target( doc_doxygen ALL
#            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
#            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#            COMMENT "Generating documentation with Doxygen"
#            VERBATIM )
#    else (DOXYGEN_FOUND)
#      message("Doxygen need to be installed to generate the documentation")
#    endif (DOXYGEN_FOUND)
#endif()
