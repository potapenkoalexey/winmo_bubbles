cmake_minimum_required(VERSION 3.10)

set(PROJECT_VERSION 2.0.1)

# project(installer VERSION ${PROJECT_VERSION})

set(WIX_INSTALLER_PLATFORM_NAME "WIN")

set(CMAKE_CURRENT_BINARY_DIR ${PROJECT_BINARY_DIR}/output)

#set(CPACK_WIX_PATCH_FILE "${PROJECT_SOURCE_DIR}/Patch.xml")
configure_file(${PROJECT_SOURCE_DIR}/templates/Variables.wxi.in ${CMAKE_CURRENT_BINARY_DIR}/VariablesOut.wxi @ONLY)
configure_file(${PROJECT_SOURCE_DIR}/Product.wxs ${CMAKE_CURRENT_BINARY_DIR}/Product.wxs @ONLY)
configure_file(${PROJECT_SOURCE_DIR}/Product.wxi ${CMAKE_CURRENT_BINARY_DIR}/Product.wxi @ONLY)

add_custom_target(installer-ALL)

message("DEBUG: CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")
add_custom_target(INSTALLER_COMPILE 
    COMMAND "candle" -arch x64 *.wxs
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Run Wix Candle to compile installer sources" )
add_custom_target(INSTALLER_LINK 
    COMMAND "light" *.wixobj -o WinMoBubbles.msi
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Run Wix Light to link and create msi" )

add_dependencies(installer-ALL INSTALLER_COMPILE INSTALLER_LINK)
