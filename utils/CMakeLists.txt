cmake_minimum_required(VERSION 3.16)

project(utils VERSION 1.0)

file(GLOB_RECURSE UTILS_SRC_FILES
    "${CMAKE_CURRENT_LIST_DIR}/*.cxx"
    )

file(GLOB_RECURSE UTILS_HEADERS_FILES
    "${CMAKE_CURRENT_LIST_DIR}/include/*.hxx"
    )

add_library(${PROJECT_NAME} 
            STATIC
            ${UTILS_SRC_FILES}
)
add_library(utils::${PROJECT_NAME}-static ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

find_package(SDL2 REQUIRED)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE "-DOM_DECLSPEC=__declspec(dllexport)")
endif(WIN32)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

if(UNIX)
    target_link_libraries(${PROJECT_NAME}
        SDL2-static
        )
elseif(WIN32)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE SDL2 
                SDL2main
        )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME utils)

# installation
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${UTILS_HEADERS_FILES}")

##### Installation targets #####
install(TARGETS utils EXPORT ${PROJECT_NAME}-targets
  LIBRARY DESTINATION "${CMAKE_BINARY_DIR}${LIB_SUFFIX}"
  ARCHIVE DESTINATION "${CMAKE_BINARY_DIR}${LIB_SUFFIX}"
  RUNTIME DESTINATION "${CMAKE_BINARY_DIR}")

##### Export files #####
if (WINDOWS)
  set(PKG_PREFIX "cmake")
else ()
  set(PKG_PREFIX "${CMAKE_BINARY_DIR}${LIB_SUFFIX}/cmake/utils")
endif ()

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/utils-config.cmake.in 
              ${CMAKE_BINARY_DIR}/utils-config.cmake
              INSTALL_DESTINATION "." )

write_basic_package_version_file("${CMAKE_BINARY_DIR}/utils-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/utils-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/utils-config-version.cmake
        DESTINATION "cmake" )

install(EXPORT ${PROJECT_NAME}-targets
  FILE ${PROJECT_NAME}-targets.cmake
  NAMESPACE utils::
  DESTINATION cmake
)

install(
  FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/utilsConfig.cmake
    ${CMAKE_BINARY_DIR}/utilsConfigVersion.cmake
  DESTINATION ${PKG_PREFIX}
  COMPONENT winmo_engine
)

install(FILES ${UTILS_HEADERS_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/utils)

