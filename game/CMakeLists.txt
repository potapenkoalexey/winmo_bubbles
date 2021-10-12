cmake_minimum_required(VERSION 3.16)

project(game)

file(GLOB_RECURSE GAME_SRC_FILES
    "${CMAKE_CURRENT_LIST_DIR}/*.hxx"
    "${CMAKE_CURRENT_LIST_DIR}/*.cxx"
    )

add_executable(${PROJECT_NAME} ${GAME_SRC_FILES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/utils/)

target_link_libraries(${PROJECT_NAME} PRIVATE ${ENGINE_LIB_NAME} utils)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${EXECUTABLE_NAME})

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/")
    file(COPY ${CMAKE_SOURCE_DIR}/thirdparty/win/opengl32.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

# for cpack
install(TARGETS ${PROJECT_NAME}
        RUNTIME
        DESTINATION .
        COMPONENT winmo_bin
)

add_dependencies(whole_project ${PROJECT_NAME})
