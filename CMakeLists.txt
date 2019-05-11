cmake_minimum_required(VERSION 3.9)
project(winmo_states)

#target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/engine")

add_executable(${PROJECT_NAME}
        src/engine/engine.hxx
        src/engine/engine.cxx
        src/game/main.cxx
        src/game/block.hxx
        src/game/block.cxx
        src/game/select_mode_state.hxx
        src/game/select_mode_state.cxx
        src/game/classic_state.cxx
        src/game/classic_state.hxx
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#find_library(SDL2_LIB NAMES libSDL2.a)
#find_library(SNDIO_LIB NAMES libsndio.so)
  
target_link_libraries(${PROJECT_NAME} 
   #"${SDL2_LIB}" # full path to libSDL2.a force to staticaly link with it
   #/usr/local/lib/libSDL2.a
   #-lSDL2main
   -lSDL2
   -lm
   -ldl
   -lpthread
   -lrt
   #"${SNDIO_LIB}"
   #/usr/lib/x86_64-linux-gnu/libsndio.so
   -lGL
   )
