[![qa](https://github.com/andrsd/exodusIIcpp/actions/workflows/qa.yml/badge.svg)](https://github.com/andrsd/exodusIIcpp/actions/workflows/qa.yml)
[![build](https://github.com/andrsd/exodusIIcpp/actions/workflows/build.yml/badge.svg)](https://github.com/andrsd/exodusIIcpp/actions/workflows/build.yml)
[![codecov](https://codecov.io/gh/andrsd/exodusIIcpp/branch/main/graph/badge.svg?token=S31RDT4OAM)](https://codecov.io/gh/andrsd/exodusIIcpp)
[![License](http://img.shields.io/:license-mit-blue.svg)](https://andrsd.mit-license.org/)
[![Scc Count Badge](https://sloc.xyz/github/andrsd/exodusIIcpp/)](https://github.com/andrsd/exodusIIcpp/)

# exodusIIcpp

`exodusIIcpp` is a C++17 wrapper around [ExodusII file format](https://sandialabs.github.io/seacas-docs/sphinx/html/index.html#exodus-library).

### Features

- C++ API for reading and writing exodusII files
- Support for:
    - element blocks,
    - side sets,
    - node sets
- CMake installation
- Support for Linux, macOS X

### Examples

Reading a file

```
#include "exodusIIcpp/exodusIIcpp.h"

exodusIIcpp::File exo("file.exo", exodusIIcpp::FileAccess::READ);
exo.read();

auto & x = exo.get_x_coords();
auto & y = exo.get_y_coords();

for (auto & eb : exo.get_element_blocks()) {
  ...
}

for (auto & ns : exo.get_node_sets()) {
  ...
}

for (auto & ss : exo.get_side_sets()) {
  ...
}
```

### Requirements

- C++17 compiler
- CMake 3.16+
- [fmt](https://github.com/fmtlib/fmt) 11.x
- ExodusII
    - usually also depends on NetCDF and HDF5
