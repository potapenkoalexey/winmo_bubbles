cmake_minimum_required(VERSION 3.9)
project(winmo_bubbles)

add_executable(${PROJECT_NAME}
        src/engine/engine.hxx
        src/engine/engine.cxx
        src/engine/math_structures.hxx
        src/engine/math_structures.cxx
        src/engine/color.hxx
        src/engine/color.cxx
        src/engine/sound_buffer.hxx
        src/engine/sound_buffer.cxx
        src/engine/random_generator.hxx
        src/engine/random_generator.cxx
        src/game/main.cxx
        src/game/components/block.hxx
        src/game/components/block.cxx
        src/game/components/field.hxx
        src/game/components/field.cxx
        src/game/components/counter.hxx
        src/game/components/counter.cxx
        src/game/components/progress_desk.hxx
        src/game/components/progress_desk.cxx
        src/game/components/number.hxx
        src/game/components/number.cxx
        src/game/states/select_mode_state.hxx
        src/game/states/select_mode_state.cxx
        src/game/states/classic_state.hxx
        src/game/states/classic_state.cxx
        src/game/states/extreme_state.hxx
        src/game/states/extreme_state.cxx
        src/game/states/level_complete_state.hxx
        src/game/states/level_complete_state.cxx
        src/game/states/game_over_state.hxx
        src/game/states/game_over_state.cxx
        src/game/global_variables.hxx
        src/game/global_variables.cxx
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME} 
   -lSDL2
   -lGL
   )
