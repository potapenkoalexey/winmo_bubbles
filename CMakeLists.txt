cmake_minimum_required(VERSION 3.10)

project(winmo_bubbles VERSION 2.1 LANGUAGES CXX)

##### turn off cmake extentions
if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

##### sdl
add_subdirectory(libs/SDL2-2.0.8)
include_directories(libs/SDL2-2.0.8/include)

##### additional sdl headers
include_directories(src/SDL2)

##### main project
file(GLOB_RECURSE GAME_SRC_FILES
    "${CMAKE_CURRENT_LIST_DIR}/src/game/*.hxx"
    "${CMAKE_CURRENT_LIST_DIR}/src/game/*.cxx"
    )

add_executable(${PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/src/game/main.cxx
    ${GAME_SRC_FILES}
    )

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

##### engine lib
file(GLOB_RECURSE ENGINE_SRC_FILES
    "${CMAKE_CURRENT_LIST_DIR}/src/engine/*.hxx"
    "${CMAKE_CURRENT_LIST_DIR}/src/engine/*.cxx"
    )

set(ENGINE_LIB_NAME engine)

add_library(${ENGINE_LIB_NAME}
    ${ENGINE_SRC_FILES}
    )

target_compile_features(${ENGINE_LIB_NAME} PUBLIC cxx_std_17)

##### linking engine lib
if (UNIX)
    target_link_libraries(${ENGINE_LIB_NAME}
        SDL2
        -lGL
        )
elseif (WIN32)
    target_link_libraries(${ENGINE_LIB_NAME}
        SDL2
        -lopengl32
        )
endif()

##### linking main project
if (UNIX)
    target_link_libraries(${PROJECT_NAME}
        ${ENGINE_LIB_NAME}
        )
elseif (WIN32)
    target_link_libraries(${PROJECT_NAME}
        ${ENGINE_LIB_NAME}
        )
endif()

##### documentation
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
