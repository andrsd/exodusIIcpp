Reading a file
==============

This example demonstrates how to read data from an ExodusII file.

Opening a file
--------------

To read an existing ExodusII file, create a ``File`` object with ``FileAccess::READ``:

.. code-block:: cpp

    #include "exodusIIcpp/exodusIIcpp.h"

    using namespace exodusIIcpp;

    // Open file for reading
    File file("mesh.e", FileAccess::READ);

The file handle will automatically initialize by calling the constructor, which opens the file and reads the metadata.

Reading mesh information
------------------------

Once the file is open, you can query basic mesh information:

.. code-block:: cpp

    // Get spatial dimension
    int dim = file.get_dim();

    // Get counts
    int num_nodes = file.get_num_nodes();
    int num_elements = file.get_num_elements();
    int num_blocks = file.get_num_element_blocks();

    // Get title
    std::string title = file.get_title();

Reading coordinates
-------------------

To read node coordinates:

.. code-block:: cpp

    // Read all coordinate data from the file
    file.read_coords();

    // Get the coordinates
    const std::vector<double> & x = file.get_x_coords();
    const std::vector<double> & y = file.get_y_coords();
    const std::vector<double> & z = file.get_z_coords();

Reading element blocks
----------------------

To read element block information:

.. code-block:: cpp

    // Read all element blocks
    file.read_blocks();

    // Get all blocks
    const std::vector<ElementBlock> & blocks = file.get_element_blocks();

    // Access individual block
    const ElementBlock & block = file.get_element_block(0);

Reading node and side sets
---------------------------

To read node and side sets:

.. code-block:: cpp

    // Read node sets
    file.read_node_sets();
    const std::vector<NodeSet> & node_sets = file.get_node_sets();

    // Read side sets
    file.read_side_sets();
    const std::vector<SideSet> & side_sets = file.get_side_sets();

    // Get side set node list
    std::vector<int> node_count_list;
    std::vector<int> node_list;
    file.get_side_set_node_list(side_set_id, node_count_list, node_list);

Reading time step data
----------------------

To read time-dependent data:

.. code-block:: cpp

    // Read time values
    file.read_times();

    // Get number of time steps and time values
    int num_times = file.get_num_times();
    const std::vector<double> & times = file.get_times();

Reading variables
-----------------

To read variable names and values:

.. code-block:: cpp

    // Get variable names
    std::vector<std::string> nodal_var_names = file.get_nodal_variable_names();
    std::vector<std::string> elem_var_names = file.get_elemental_variable_names();
    std::vector<std::string> global_var_names = file.get_global_variable_names();

    // Read nodal variable values for a specific time step and variable
    int time_step = 1;  // 1-based indexing
    int var_index = 1;  // 1-based indexing
    std::vector<double> values = file.get_nodal_variable_values(time_step, var_index);

    // Read elemental variable values for a specific block
    int block_id = 1;
    std::vector<double> elem_values = file.get_elemental_variable_values(time_step, var_index, block_id);

    // Read global variable values
    std::vector<double> global_values = file.get_global_variable_values(time_step);

Reading all data at once
------------------------

For convenience, you can read all data in one call:

.. code-block:: cpp

    // Read all mesh and variable data
    file.read();

Closing the file
----------------

The file is automatically closed when the ``File`` object goes out of scope. You can also explicitly close it:

.. code-block:: cpp

    file.close();

Complete example
----------------

Here's a complete example that opens a file, reads mesh and time step data, and prints information:

.. code-block:: cpp

    #include "exodusIIcpp/exodusIIcpp.h"
    #include <iostream>

    using namespace exodusIIcpp;

    int main()
    {
        // Open file
        File file("mesh.e", FileAccess::READ);

        // Print basic info
        std::cout << "Title: " << file.get_title() << std::endl;
        std::cout << "Dimension: " << file.get_dim() << std::endl;
        std::cout << "Nodes: " << file.get_num_nodes() << std::endl;
        std::cout << "Elements: " << file.get_num_elements() << std::endl;

        // Read and print time steps
        file.read_times();
        std::cout << "Time steps: " << file.get_num_times() << std::endl;

        // Read nodal variable names
        std::vector<std::string> var_names = file.get_nodal_variable_names();
        for (size_t i = 0; i < var_names.size(); ++i) {
            std::cout << "Variable " << (i + 1) << ": " << var_names[i] << std::endl;
        }

        // Read and print first variable at first time step
        if (var_names.size() > 0 && file.get_num_times() > 0) {
            std::vector<double> values = file.get_nodal_variable_values(1, 1);
            std::cout << "First 5 values of first variable: ";
            for (size_t i = 0; i < std::min(values.size(), size_t(5)); ++i) {
                std::cout << values[i] << " ";
            }
            std::cout << std::endl;
        }

        return 0;
    }
