project(exodusIIcpp-test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME}
    ElementBlock_test.cpp
    Error_test.cpp
    File_test.cpp
    NodeSet_test.cpp
    SideSet_test.cpp
    main.cpp
)
target_code_coverage(${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${PROJECT_BINARY_DIR}
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        exodusIIcpp
        gtest_main
        gmock_main
)

set(EXODUSIICPP_UNIT_TEST_ASSETS ${PROJECT_SOURCE_DIR}/assets CACHE PATH "" FORCE)
mark_as_advanced(FORCE EXODUSIICPP_UNIT_TEST_ASSETS)

configure_file(config.h.in config.h)

add_test(
    NAME ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
)
if(EXODUSIICPP_CODE_COVERAGE)
    set_tests_properties(
        ${PROJECT_NAME}
        PROPERTIES
            ENVIRONMENT LLVM_PROFILE_FILE=${PROJECT_NAME}.profraw
    )
endif()
