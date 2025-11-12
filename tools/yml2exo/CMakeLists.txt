project(yml2exo LANGUAGES CXX)

find_package(yaml-cpp 0.8 REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/contrib
        ${CMAKE_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/..
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        fmt::fmt
        yaml-cpp::yaml-cpp
        exodusIIcpp
)

if (EXODUSIICPP_INSTALL)
    install(
        TARGETS ${PROJECT_NAME}
        EXPORT exodusIIcppTargets
        RUNTIME DESTINATION bin
    )
endif()
