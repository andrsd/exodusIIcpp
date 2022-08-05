# Documentation

find_package(Doxygen QUIET)

if(DOXYGEN_FOUND)
    set(DOXYGEN_GENERATE_HTML NO)
    set(DOXYGEN_GENERATE_XML YES)
    set(DOXYGEN_STRIP_FROM_INC_PATH
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
    )
    set(DOXYGEN_STRIP_FROM_PATH
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
    )

    doxygen_add_docs(
        doxygen
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
        STRIP_FROM_PATH
        COMMENT "Generate doxygen XML"
    )

    find_program(SPHINX_BUILD sphinx-build)
    mark_as_advanced(FORCE
        SPHINX_BUILD
    )

    add_custom_target(doc DEPENDS ${PROJECT_BINARY_DIR}/docs/html/index.html)

    add_custom_command(
        OUTPUT
            ${PROJECT_BINARY_DIR}/docs/html/index.html
        COMMAND
            ${SPHINX_BUILD} -b html -d ${PROJECT_BINARY_DIR}/docs/.doctrees ${PROJECT_SOURCE_DIR}/docs html
        DEPENDS
            ${PROJECT_SOURCE_DIR}/docs/conf.py
            ${PROJECT_BINARY_DIR}/docs/xml/index.xml
    )

    add_custom_command(
        OUTPUT
            ${PROJECT_BINARY_DIR}/docs/xml/index.xml
        COMMAND
            doxygen Doxyfile.doxygen
        DEPENDS
            ${PROJECT_BINARY_DIR}/docs/Doxyfile.doxygen
            ${PROJECT_SOURCE_DIR}/include/*.h
    )

    add_custom_command(
        TARGET doc
        POST_BUILD
        COMMAND ;
        COMMENT
            "Open ${PROJECT_BINARY_DIR}/docs/html/index.html in your browser to view the documentation."
    )

endif()
