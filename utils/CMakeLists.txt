cmake_minimum_required(VERSION 3.16)

project(utils)

file(GLOB_RECURSE UTILS_SRC_FILES
    "${CMAKE_CURRENT_LIST_DIR}/*.hxx"
    "${CMAKE_CURRENT_LIST_DIR}/*.cxx"
    )

add_library(${PROJECT_NAME} 
            STATIC
            ${UTILS_SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/libs/SDL2-2.0.16/include/)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE "-DOM_DECLSPEC=__declspec(dllexport)")
endif(WIN32)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if(UNIX)
    target_link_libraries(${PROJECT_NAME}
        SDL2-static
        )
elseif(WIN32)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE SDL2 
                SDL2main
        )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME utils)

# for cpack
install(TARGETS ${PROJECT_NAME}
        OBJECTS
        DESTINATION .
        COMPONENT winmo_engine
)