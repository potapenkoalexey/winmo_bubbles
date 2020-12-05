cmake_minimum_required(VERSION 3.16)

set(DOC_CHM_NAME ${GAME_NAME})

# documentation build as an option and set it to ON by default
option(BUILD_DOC "Build documentation" ON)
find_package(Doxygen)
if (DOXYGEN_FOUND)

    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    message(STATUS "Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating documentation with Doxygen"
        VERBATIM )
    # add_dependencies(build-ALL doc_doxygen)

else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the documentation")
endif (DOXYGEN_FOUND)
