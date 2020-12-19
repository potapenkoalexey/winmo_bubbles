cmake_minimum_required(VERSION 3.16)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/data
        DESTINATION ${GAME_INSTALL_PATH}
        COMPONENT winmo_unix_data
)

# install(PROGRAMS ${CMAKE_BINARY_DIR}/bin/run_rpcserver.sh DESTINATION bin COMPONENT RPCSERVER)
# install(FILES ${CMAKE_BINARY_DIR}/config/LinuxEcatKPAMaster.ini DESTINATION config COMPONENT MRT)