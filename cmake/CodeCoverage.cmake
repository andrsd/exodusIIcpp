if(CMAKE_PROJECT_NAME STREQUAL "exodusIIcpp")

option(EXODUSIICPP_CODE_COVERAGE "Builds targets with code coverage instrumentation" OFF)

if(EXODUSIICPP_CODE_COVERAGE)

    set(COVERAGE_INFO ${PROJECT_BINARY_DIR}/coverage.info)
    mark_as_advanced(FORCE
        COVERAGE_INFO
    )
    set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${COVERAGE_INFO}")

    if(CMAKE_C_COMPILER_ID MATCHES "(Apple)?[Cc]lang" OR CMAKE_CXX_COMPILER_ID MATCHES "(Apple)?[Cc]lang")
        find_program(
            LLVM_COV_PATH
            NAMES
                llvm-cov
                llvm-cov-12
                llvm-cov-13
                llvm-cov-14
        )
        find_program(
            LLVM_PROFDATA_PATH
            NAMES
                llvm-profdata
                llvm-profdata-12
                llvm-profdata-13
                llvm-profdata-14
        )
        mark_as_advanced(FORCE
            LLVM_COV_PATH
            LLVM_PROFDATA_PATH
        )

        set(EXCLUDE_REGEX
            --ignore-filename-regex=/include/fmt/*
            --ignore-filename-regex=/include/gtest/*
            --ignore-filename-regex=/include/gmock/*
            --ignore-filename-regex=test/*
            --ignore-filename-regex=exodusII.h
            --ignore-filename-regex=mpicxx.h
        )

        set(CODE_COVERAGE_BINS
            --object=${PROJECT_BINARY_DIR}/test/exodusIIcpp-test
            --object=${PROJECT_BINARY_DIR}/libexodusIIcpp${CMAKE_SHARED_LIBRARY_SUFFIX}
        )

        set(MERGED_PROFDATA ${PROJECT_BINARY_DIR}/all-merged.profdata)


        add_custom_target(coverage DEPENDS ${COVERAGE_INFO})

        add_custom_command(
            OUTPUT
                ${COVERAGE_INFO}
            COMMAND
                "${LLVM_COV_PATH}"
                export
                ${CODE_COVERAGE_BINS}
                -instr-profile=${MERGED_PROFDATA}
                -format="lcov"
                ${EXCLUDE_REGEX}
                > ${COVERAGE_INFO}
            DEPENDS
                ${MERGED_PROFDATA}
        )

        add_custom_command(
            OUTPUT
                ${MERGED_PROFDATA}
            COMMAND
                ${LLVM_PROFDATA_PATH}
                merge
                -sparse
                ${CMAKE_BINARY_DIR}/test/exodusIIcpp-test-*.profraw
                -o ${MERGED_PROFDATA}
        )

        add_custom_target(htmlcov DEPENDS ${PROJECT_BINARY_DIR}/htmlcov/index.html)
        add_custom_command(
            OUTPUT
                ${PROJECT_BINARY_DIR}/htmlcov/index.html
            COMMAND
                ${LLVM_COV_PATH}
                show
                ${CODE_COVERAGE_BINS}
                -instr-profile=${MERGED_PROFDATA}
                -show-line-counts-or-regions
                -output-dir=${CMAKE_BINARY_DIR}/htmlcov
                -format="html"
                ${EXCLUDE_REGEX}
            DEPENDS
                ${COVERAGE_INFO}
        )

        add_custom_command(
            TARGET htmlcov
            POST_BUILD
            COMMAND ;
            COMMENT
                "Open ${PROJECT_BINARY_DIR}/htmlcov/index.html in your browser to view the coverage report."
        )

        function(target_code_coverage TARGET_NAME)
            target_compile_options(${TARGET_NAME} PUBLIC -fprofile-instr-generate -fcoverage-mapping)
            target_link_options(${TARGET_NAME} PUBLIC -fprofile-instr-generate -fcoverage-mapping)
        endfunction()

    elseif(CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        find_program(GCOV_PATH NAMES gcov)
        find_program(LCOV_PATH lcov)
        find_program(GENHTML_PATH genhtml)
        mark_as_advanced(FORCE
            GCOV_PATH
            LCOV_PATH
            GENHTML_PATH
        )

        set(EXCLUDE_REGEX
            --exclude=*/include/gtest/*
            --exclude=*/include/gmock/*
            --exclude=*/include/fmt/*
            --exclude=/usr/include/*
            --exclude=exodusII.h
            --exclude=mpicxx.h
        )

        add_custom_target(coverage DEPENDS ${COVERAGE_INFO})

        add_custom_command(
            OUTPUT
                ${COVERAGE_INFO}
            COMMAND
                ${LCOV_PATH}
                --capture
                --directory ${PROJECT_BINARY_DIR}
                --output-file ${COVERAGE_INFO}
                ${EXCLUDE_REGEX}
        )

        add_custom_target(htmlcov DEPENDS ${PROJECT_BINARY_DIR}/htmlcov/index.html)
        add_custom_command(
            OUTPUT
                ${PROJECT_BINARY_DIR}/htmlcov/index.html
            COMMAND
                ${GENHTML_PATH}
                --output-directory=${CMAKE_BINARY_DIR}/htmlcov
                ${COVERAGE_INFO}
            DEPENDS
                ${COVERAGE_INFO}
        )

        add_custom_command(
            TARGET htmlcov
            POST_BUILD
            COMMAND ;
            COMMENT
                "Open ${PROJECT_BINARY_DIR}/htmlcov/index.html in your browser to view the coverage report."
        )

        function(target_code_coverage TARGET_NAME)
            target_compile_options(${TARGET_NAME} PUBLIC -fprofile-arcs -ftest-coverage)
            target_link_options(${TARGET_NAME} PUBLIC -fprofile-arcs -ftest-coverage)
        endfunction()

    else()
        message(STATUS, "Code coverage for your compiler (${CMAKE_C_COMPILER_ID}) is not supported.")
    endif()

else()

    function(target_code_coverage TARGET_NAME)
    endfunction()

endif()

endif()
