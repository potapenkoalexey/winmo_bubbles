cmake_minimum_required(VERSION 3.6)

project(GAME)

# armeabi-v7a requires cpufeatures library
# include(AndroidNdkModules)
# android_ndk_import_module_cpufeatures()


# SDL sources are in a subfolder named "SDL"
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/SDL)
include_directories(${CMAKE_CURRENT_LIST_DIR}/SDL/include)

# Compilation of companion libraries
# add_subdirectory(SDL2_image)
# add_subdirectory(SDL2_mixer)
# add_subdirectory(SDL2_ttf)

# Your game and its CMakeLists.txt are in a subfolder named "src"
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src)