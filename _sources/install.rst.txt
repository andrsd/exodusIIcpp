Installation
============

Requirements

* C++17 compiler
* cmake 3.16+
* `fmt`_
* ExodusII

   * Usually depends on NetCDF and HDF5

.. _fmt: https://github.com/fmtlib/fmt

CMake
-----

Standard installation steps should build the library:

.. code-block:: shell

   $ cd /path/to/exodusIIcpp
   $ mkdir build
   $ cd build
   $ cmake ..
   $ make
   $ make install


Embedding into project
----------------------

* Copy files from ``include/`` and ``src/`` directory into your project directory
* Add ``CMakeLists.txt`` file in that directory:

   .. code-block:: cmake

      project(exodusIIcpp)

      add_library(${PROJECT_NAME}
         STATIC
             exodusIIcppElementBlock.cpp
             exodusIIcppFile.cpp
             exodusIIcppNodeSet.cpp
             exodusIIcppSideSet.cpp
      )

      target_link_libraries(
         ${PROJECT_NAME}
         PUBLIC
             ${EXODUSII_LIBRARY}
      )

      install(
         DIRECTORY
             ${PROJECT_SOURCE_DIR}
         DESTINATION
             ${CMAKE_INSTALL_INCLUDEDIR}
         FILES_MATCHING
             PATTERN "*.h"
      )

* Copy the ``FindExodsuII.cmake`` and ``FindNetCDF.cmake`` files from the ``cmake/`` directory into your project

* Use the following code to find the dependencies

   .. code-block:: cmake

      find_package(NetCDF REQUIRED)
      find_package(ExodusII REQUIRED)
      find_package(HDF5 COMPONENTS C)

   .. note::

      ExodusII depends on NetCDF. Newer versions also depend on HDF5.

   This will set the ``EXODUSII_LIBRARY`` variable we are using in the ``CMakeLists.txt`` above.

* Do not forget to add your directory in your main ``CMakeLists.txt`` via:

   .. code-block:: cmake

      add_subdirectory(path/to/the/dir/with/exodusIIcpp/files)
