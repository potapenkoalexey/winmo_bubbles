cmake_minimum_required(VERSION 3.9)

project(winmo_bubbles)

add_executable(${PROJECT_NAME}
        ${CMAKE_CURRENT_LIST_DIR}/src/engine/engine.hxx
        ${CMAKE_CURRENT_LIST_DIR}/src/engine/engine.cxx
        ${CMAKE_CURRENT_LIST_DIR}/src/engine/math_structures.hxx
        ${CMAKE_CURRENT_LIST_DIR}/src/engine/math_structures.cxx
        ${CMAKE_CURRENT_LIST_DIR}/src/engine/color.hxx
        ${CMAKE_CURRENT_LIST_DIR}/src/engine/color.cxx
        ${CMAKE_CURRENT_LIST_DIR}/src/engine/sound_buffer.hxx
        ${CMAKE_CURRENT_LIST_DIR}/src/engine/sound_buffer.cxx
        ${CMAKE_CURRENT_LIST_DIR}/src/engine/random_generator.hxx
        ${CMAKE_CURRENT_LIST_DIR}/src/engine/random_generator.cxx
        ${CMAKE_CURRENT_LIST_DIR}/src/engine/game_state.hxx
        ${CMAKE_CURRENT_LIST_DIR}/src/engine/game_state.cxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/main.cxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/components/block.hxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/components/block.cxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/components/field.hxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/components/field.cxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/components/counter.hxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/components/counter.cxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/components/progress_desk.hxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/components/progress_desk.cxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/components/number.hxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/components/number.cxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/states/select_mode_state.hxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/states/select_mode_state.cxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/states/classic_state.hxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/states/classic_state.cxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/states/extreme_state.hxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/states/extreme_state.cxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/states/level_complete_state.hxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/states/level_complete_state.cxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/states/game_over_state.hxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/states/game_over_state.cxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/global_variables.hxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/global_variables.cxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/settings.hxx
        ${CMAKE_CURRENT_LIST_DIR}/src/game/settings.cxx
)

target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/src")

#check this instruction work
target_link_options(${PROJECT_NAME} PRIVATE -static)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if (UNIX)
#add_library(sdl STATIC IMPORTED)
#set_property(TARGET sdl PROPERTY
#        IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/libs/unix/SDL2.a)
target_link_libraries(${PROJECT_NAME}
   -lSDL2
   #-sdl
   -lGL
   )
endif()

if (WIN32)
add_library(sdl STATIC IMPORTED)
set_property(TARGET sdl PROPERTY
        IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/libs/win/SDL2.dll)

target_link_libraries(${PROJECT_NAME}
    sdl
    -lopengl32
    )
endif()
