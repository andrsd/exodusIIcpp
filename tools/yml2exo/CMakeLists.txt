project(yml2exo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(yaml-cpp 0.7 REQUIRED)

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
        yaml-cpp
        exodusIIcpp
)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT exodusIIcppTargets
    RUNTIME DESTINATION bin
)
