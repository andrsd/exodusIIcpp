project(exodusIIcpp-test)

include(GoogleTest)

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

set(EXODUSIICPP_UNIT_TEST_ASSETS ${PROJECT_SOURCE_DIR}/assets CACHE PATH "" FORCE)
mark_as_advanced(FORCE EXODUSIICPP_UNIT_TEST_ASSETS)

configure_file(config.h.in config.h)

gtest_discover_tests(${PROJECT_NAME})
