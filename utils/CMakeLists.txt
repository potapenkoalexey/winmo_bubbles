cmake_minimum_required(VERSION 3.16)

project(utils VERSION 1.0)

file(GLOB_RECURSE UTILS_SRC_FILES
    "${CMAKE_CURRENT_LIST_DIR}/*.cxx"
    )

file(GLOB_RECURSE UTILS_HEADERS_FILES
    "${CMAKE_CURRENT_LIST_DIR}/include/*.hxx"
    )

add_library(${PROJECT_NAME} 
            SHARED 
            ${UTILS_SRC_FILES}
)
add_library(utils::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE "-DOM_DECLSPEC=__declspec(dllexport)")
endif(WIN32)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if(UNIX)
    target_link_libraries(${PROJECT_NAME}
        SDL2
        )
elseif(WIN32)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE SDL2 
                SDL2main
        )
endif()

# attach the "include" directory to the target
target_include_directories(${PROJECT_NAME} PUBLIC include)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME utils)

# set version
get_property(major GLOBAL PROPERTY PROJECT_VERSION_MAJOR)
get_property(minor GLOBAL PROPERTY PROJECT_VERSION_MINOR)
get_property(patch GLOBAL PROPERTY PROJECT_VERSION_PATCH)

set(PROJECT_VERSION ${major}.${minor}.${patch})

set_target_properties(${PROJECT_NAME} PROPERTIES 
        VERSION ${PROJECT_VERSION} 
        SOVERSION ${major}.${minor})

add_dependencies(whole_project ${PROJECT_NAME})
