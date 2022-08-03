project(exodusIIcpp-test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME}
    main.cpp
)
target_code_coverage(${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        exodusIIcpp
        gtest_main
        gmock_main
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
