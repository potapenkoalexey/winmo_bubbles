cmake_minimum_required(VERSION 3.16)

project(${ENGINE_LIB_NAME})

file(GLOB_RECURSE ENGINE_SRC_FILES
    "${CMAKE_SOURCE_DIR}/game/global_variables.hxx"
    "${CMAKE_SOURCE_DIR}/game/global_variables.cxx"
    "${CMAKE_CURRENT_LIST_DIR}/include/*.hxx"
    "${CMAKE_CURRENT_LIST_DIR}/*.cxx"
)

file(GLOB_RECURSE ENGINE_HRDS_FILES
    "${CMAKE_CURRENT_LIST_DIR}/include/*.hxx"
)

add_library(${PROJECT_NAME} STATIC ${ENGINE_SRC_FILES})

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE "-DOM_DECLSPEC=__declspec(dllexport)")
endif(WIN32)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if (UNIX)
    set(ENGINE_LINK_LIBS
        utils
        SDL2
        SDL2main
        # -lGL
        EGL
        GLESv2
    )
endif()
if (ANDROID)
    set(ENGINE_LINK_LIBS
        utils
        SDL2
        SDL2main
        -ldl
        # -lGL
        EGL
        GLESv2
        android
        OpenSLES
    )
endif()
if (WIN32)
    set(ENGINE_LINK_LIBS
        SDL2
        SDL2main
        opengl32
    )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${ENGINE_LINK_LIBS})

# attach the "include" directory to the target
target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${CMAKE_SOURCE_DIR}/src"
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${ENGINE_LIB_NAME})

# for cpack
install(TARGETS ${PROJECT_NAME}
    OBJECTS
        DESTINATION . COMPONENT winmo_engine
)

# set version
get_property(major GLOBAL PROPERTY PROJECT_VERSION_MAJOR)
get_property(minor GLOBAL PROPERTY PROJECT_VERSION_MINOR)
get_property(patch GLOBAL PROPERTY PROJECT_VERSION_PATCH)

set(PROJECT_VERSION ${major}.${minor}.${patch})

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${major}.${minor}
)
