project(pyexodusIIcpp LANGUAGES C CXX)

find_package(pybind11 2.9 REQUIRED)

pybind11_add_module(pyexodusIIcpp exodusIIcpp.cpp)

set_target_properties(pyexodusIIcpp PROPERTIES OUTPUT_NAME exodusIIcpp)

target_include_directories(
    pyexodusIIcpp
    PRIVATE
        ${CMAKE_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}/include
        ${PROJECT_BINARY_DIR}
)

target_link_libraries(
    pyexodusIIcpp
    PUBLIC
        exodusIIcpp
)

configure_file(version.h.in version.h)

set(PYTHON_SITE lib/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages)

install(
    TARGETS pyexodusIIcpp
    COMPONENT python
    LIBRARY DESTINATION ${PYTHON_SITE}/exodusIIcpp
)

install(
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/exodusIIcpp/__init__.py
    DESTINATION ${PYTHON_SITE}/exodusIIcpp
    COMPONENT python
)
