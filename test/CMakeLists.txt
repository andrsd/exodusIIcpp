project(exodusIIcpp-test)

include(GoogleTest)

add_executable(${PROJECT_NAME}
    ElementBlock_test.cpp
    Error_test.cpp
    File_test.cpp
    NodeSet_test.cpp
    SideSet_test.cpp
    main.cpp
)
target_code_coverage(${PROJECT_NAME})
target_sanitization(${PROJECT_NAME})

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

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        -DEXODUSIICPP_UNIT_TEST_ASSETS="${EXODUSIICPP_UNIT_TEST_ASSETS}"
)

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
