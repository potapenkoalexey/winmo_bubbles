cmake_minimum_required(VERSION 3.17)

project(game)

file(GLOB_RECURSE GAME_SRC_FILES
    "${CMAKE_CURRENT_LIST_DIR}/*.hxx"
    "${CMAKE_CURRENT_LIST_DIR}/*.cxx"
    )

add_executable(${PROJECT_NAME}
    ${GAME_SRC_FILES}
    )

# target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/${ENGINE_LIB_NAME})
include_directories(${CMAKE_SOURCE_DIR}/engine)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME} ${ENGINE_LIB_NAME})

# target and dependencies
# add_custom_target(${PROJECT_NAME})
# add_dependencies(build-ALL ${PROJECT_NAME})