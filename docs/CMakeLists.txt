cmake_minimum_required(VERSION 3.16)

set(DOC_CHM_NAME ${GAME_NAME})

set(CMAKE_CURRENT_BINARY_DIR ${PROJECT_BINARY_DIR}/docs/output)

# documentation build as an option and set it to ON by default
find_package(Doxygen)

if (DOXYGEN_FOUND)

    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    message(STATUS "Doxygen build started")

    add_custom_target(doc_doxygen 
                    cmake -E env
                    ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}" 
                    COMMENT "Generating ${GAME_NAME} documentation"
    )
else()
    message(WARNING "Doxygen need to be installed to generate the documentation")
endif()