cmake_minimum_required(VERSION 3.16)

project(game)

file(GLOB_RECURSE GAME_SRC_FILES
    "${CMAKE_CURRENT_LIST_DIR}/*.hxx"
    "${CMAKE_CURRENT_LIST_DIR}/*.cxx"
    "${CMAKE_SOURCE_DIR}/engine/*h.cxx"
    )

add_executable(${PROJECT_NAME}
    ${GAME_SRC_FILES}
    )

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME} engine)

set(EXECUTABLE_NAME "WinMoBubbles")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${EXECUTABLE_NAME})

# for cpack
install(TARGETS ${PROJECT_NAME}
        RUNTIME
        #DESTINATION / #../${GAME_INSTALL_PATH}
        COMPONENT winmo_bin
)

# install(FILES ${CMAKE_BINARY_DIR}/bin/${EXECUTABLE_NAME}
#         #DESTINATION /
#         #[PERMISSIONS permissions...]
#         COMPONENT winmo_bin
#         #[RENAME <name>] [OPTIONAL] [EXCLUDE_FROM_ALL]
#         )

# target and dependencies
# add_custom_command(${PROJECT_NAME}
#         COMMAND cmake --build . --target ${PROJECT_NAME}
#         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#         COMMENT "Build ${PROJECT_NAME}"
#         VERBATIM
#         )
# add_dependencies(BUILD-ALL ${PROJECT_NAME})
