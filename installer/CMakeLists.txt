cmake_minimum_required(VERSION 3.10)

project(installer VERSION ${PROJECT_VERSION})

set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/out)

configure_file(${CMAKE_CURRENT_LIST_DIR}/Variables.wxi.in ${CMAKE_CURRENT_BINARY_DIR}/VariablesOut.wxi @ONLY)

##### build the installer
#cmake -E ""%WIX%bin\candle" -arch x64 *.wxs"
#cmake -E ""%WIX%bin\light" *.wixobj -o ${GAME_NAME}.msi"