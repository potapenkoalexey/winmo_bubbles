cmake_minimum_required(VERSION 3.16)

if($ENV{WIX} EQUAL "")
    message(FATAL_ERROR "Wix toolset not found. Please, install it.")
endif()

project(installer_win VERSION ${PROJECT_VERSION})

set(WIX_INSTALLER_PLATFORM_NAME "WIN")

set(CMAKE_CURRENT_BINARY_DIR ${PROJECT_BINARY_DIR}/output)
set(RESOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/Resources/)

configure_file(${PROJECT_SOURCE_DIR}/Variables.wxi ${CMAKE_CURRENT_BINARY_DIR}/VariablesOut.wxi @ONLY)
configure_file(${PROJECT_SOURCE_DIR}/Product.wxs ${CMAKE_CURRENT_BINARY_DIR}/ProductOut.wxs @ONLY)
configure_file(${PROJECT_SOURCE_DIR}/Product.wxi ${CMAKE_CURRENT_BINARY_DIR}/ProductOut.wxi @ONLY)
configure_file(${PROJECT_SOURCE_DIR}/Features.wxi ${CMAKE_CURRENT_BINARY_DIR}/FeaturesOut.wxi @ONLY)

# linking with WixUIExtension.dll
# find_file(PATH_WixUIExtension WixUIExtension.dll PATHS ENV WIX)

add_custom_target(installer-win-ALL)

message(STATUS "Installer Binary Dir = ${CMAKE_CURRENT_BINARY_DIR}")

# glob all files from /data dir and link it with ComponentGroup and folder on target PC
add_custom_target(INSTALLER_GLOB_DATA_FILES 
    COMMAND "heat" dir "${CMAKE_SOURCE_DIR}/data/" -o "${CMAKE_CURRENT_BINARY_DIR}/DataFolderGlob.wxs" -gg -dr INSTALLLOCATION -cg Component_DATA -var wix.SourceDir=${CMAKE_SOURCE_DIR}/data/
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "-- Running Wix Heat to glob files from /data folder"
)
add_custom_target(INSTALLER_COMPILE 
    COMMAND "candle" -arch x64 -ext WixUtilExtension *.wxs
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "-- Running Wix Candle to compile installer sources"
)
add_custom_target(INSTALLER_LINK 
    COMMAND "light" *.wixobj -ext WixUIExtension -ext WixUtilExtension -out ${GAME_NAME}.msi
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "-- Running Wix Light to link and create msi"
)

# runs targets in turn
add_dependencies(INSTALLER_COMPILE INSTALLER_GLOB_DATA_FILES)
add_dependencies(INSTALLER_LINK INSTALLER_COMPILE)

add_dependencies(installer-win-ALL INSTALLER_GLOB_DATA_FILES INSTALLER_COMPILE INSTALLER_LINK)
