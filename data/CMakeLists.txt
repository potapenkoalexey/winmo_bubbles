cmake_minimum_required(VERSION 3.16)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/data
        DESTINATION ${CMAKE_INSTALL_BINDIR}/${GAME_NAME} # GNU /bin folder
        COMPONENT winmo_unix_all
)