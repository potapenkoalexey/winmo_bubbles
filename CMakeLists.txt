cmake_minimum_required(VERSION 3.9)
project(winmo_states)

#target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/engine")

add_executable(${PROJECT_NAME}
        src/engine/engine.hxx
        src/engine/engine.cxx
        src/engine/math_structures.hxx
        src/engine/math_structures.cxx
        src/engine/color.hxx
        src/engine/color.cxx
        src/engine/sound_buffer.hxx
        src/engine/sound_buffer.cxx
        src/game/main.cxx
        src/game/block.hxx
        src/game/block.cxx
        src/game/field.hxx
        src/game/field.cxx
        src/game/counter.hxx
        src/game/counter.cxx
        src/game/progress_desk.hxx
        src/game/progress_desk.cxx
        src/game/select_mode_state.hxx
        src/game/select_mode_state.cxx
        src/game/classic_state.hxx
        src/game/classic_state.cxx
        src/game/extreme_state.hxx
        src/game/extreme_state.cxx
        src/game/level_complete_state.hxx
        src/game/level_complete_state.cxx
        src/game/game_over_state.hxx
        src/game/game_over_state.cxx
        src/game/global_variables.hxx
        src/game/global_variables.cxx
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME} 
   -lSDL2
   -lm
   -ldl
   -lpthread
   -lrt
   -lGL
   )
