cmake_minimum_required(VERSION 3.16)

project(data)

# for cpack
install(DIRECTORY ${CMAKE_SOURCE_DIR}/data
        DESTINATION data/ #./${GAME_INSTALL_PATH}
        COMPONENT winmo_data
)

# install(PROGRAMS ${CMAKE_BINARY_DIR}/bin/rpcserver.sh DESTINATION bin COMPONENT RPCSERVER)
# install(FILES ${CMAKE_BINARY_DIR}/config/***.ini DESTINATION config COMPONENT MRT)