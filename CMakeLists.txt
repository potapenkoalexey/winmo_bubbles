cmake_minimum_required(VERSION 3.10)

set(GAME_NAME WinMoBubbles)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(PROJECT_VERSION_MAJOR 2)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

project(${GAME_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin")

##### configure build
set(CMAKE_SUPPRESS_REGENERATION true)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
add_definitions( -D_CRT_SECURE_NO_WARNINGS )

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
  message(STATUS "Added parallel build arguments to CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
  message(STATUS "Added parallel build arguments to CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
endif()

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
    ${GAME_SRC_FILES}
    )

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

##### engine lib
set(ENGINE_LIB_NAME engine)

file(GLOB_RECURSE ENGINE_SRC_FILES
    "${CMAKE_CURRENT_LIST_DIR}/src/engine/*.hxx"
    "${CMAKE_CURRENT_LIST_DIR}/src/engine/*.cxx"
    )

add_library(${ENGINE_LIB_NAME}
    ${ENGINE_SRC_FILES}
    )

if(WIN32)   
  target_compile_definitions(${ENGINE_LIB_NAME} PRIVATE "-DOM_DECLSPEC=__declspec(dllexport)")
endif(WIN32)

target_compile_features(${ENGINE_LIB_NAME} PUBLIC cxx_std_17)

##### linking engine lib
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

##### linking main project
target_link_libraries(${PROJECT_NAME} ${ENGINE_LIB_NAME})

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

##### windows installer
# if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
        if(WIN32)
            # installer build as an option and set it to ON by default
            option(BUILD_INSTALLER "Build installer" ON)
            message(STATUS "Installer build started")
            add_subdirectory(installer)
        endif()
# endif()

##### build all
#cmake -E cmake --build . --config Release

add_custom_target(build-ALL ALL)
add_dependencies(build-ALL ${PROJECT_NAME} ${ENGINE_LIB_NAME})