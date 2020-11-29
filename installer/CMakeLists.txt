cmake_minimum_required(VERSION 3.10)

set(PROJECT_VERSION 2.0.1)

project(installer VERSION ${PROJECT_VERSION})

set(WIX_INSTALLER_PLATFORM_NAME "WIN")

set(CMAKE_CURRENT_BINARY_DIR ${PROJECT_BINARY_DIR}/out)

#set(CPACK_WIX_PATCH_FILE "${PROJECT_SOURCE_DIR}/Patch.xml")
configure_file(${PROJECT_SOURCE_DIR}/templates/Variables.wxi.in ${CMAKE_CURRENT_BINARY_DIR}/VariablesOut.wxi @ONLY)
configure_file(${PROJECT_SOURCE_DIR}/Product.wxs ${CMAKE_CURRENT_BINARY_DIR}/Product.wxs @ONLY)
configure_file(${PROJECT_SOURCE_DIR}/Product.wxi ${CMAKE_CURRENT_BINARY_DIR}/Product.wxi @ONLY)

# add_custom_target(installer-ALL)
# add_dependencies(installer-ALL installer)

##### build the installer
#cmake -E ""%WIX%bin\candle" -arch x64 *.wxs"
#cmake -E ""%WIX%bin\light" *.wixobj -o ${GAME_NAME}.msi"

#  add_custom_target(INSTALLER_COMPILE COMMAND "%WIX%bin/candle" -arch x64 *.wxs)
# execute_process(COMMAND  "%WIX%bin/candle" -arch x64 *.wxs OUTPUT_VARIABLE INSTALLER_COMPILE)
# string(STRIP ${INSTALLER_COMPILE} INSTALLER_COMPILE)
# message(STATUS "-- INSTALLER COMPILE")

#  add_custom_target(INSTALLER_BUILD COMMAND "%WIX%bin/light" *.wixobj -o ${GAME_NAME}.msi)
# execute_process(COMMAND "%WIX%bin/light" *.wixobj -o ${GAME_NAME}.msi OUTPUT_VARIABLE INSTALLER_BUILD)
# string(STRIP ${INSTALLER_BUILD} INSTALLER_BUILD)
# message(STATUS "-- INSTALLER BUILD")

message("DEBUG: CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")
execute_process(COMMAND "%WIX%bin/candle" -arch x64 ${CMAKE_CURRENT_BINARY_DIR}/*.wxs)
execute_process(COMMAND "%WIX%bin/light" *.wixobj -o ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.msi)
