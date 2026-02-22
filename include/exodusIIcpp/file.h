// SPDX-FileCopyrightText: 2022 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <vector>
#include <map>
#include "element_block.h"
#include "enums.h"
#include "error.h"
#include "node_set.h"
#include "side_set.h"
#include "filesystem.h"

namespace exodusIIcpp {

class File {
protected:
    /// File open/create policy
    exodusIIcpp::FileAccess file_access;
    /// Size
    int cpu_word_size;
    /// Size
    int io_word_size;
    /// Version
    float version;
    /// Title
    std::string title;
    /// ExodusII file handle
    int exoid;
    /// Number of spatial dimensions
    int n_dim;
    /// Number of nodes
    int n_nodes;
    /// Number of elements
    int n_elems;
    /// Number of element blocks
    int n_elem_blks;
    /// Number of node sets
    int n_node_sets;
    /// Number of side sets
    int n_side_sets;
    /// x-coordinates
    std::vector<double> x;
    /// y-coordinates
    std::vector<double> y;
    /// z-coordinates
    std::vector<double> z;
    /// Coordinate names
    std::vector<std::string> coord_names;
    /// Element map
    std::vector<int> elem_map;
    /// Element blocks
    std::vector<ElementBlock> element_blocks;
    /// Face sets
    std::vector<SideSet> side_sets;
    /// Node sets
    std::vector<NodeSet> node_sets;
    /// Times
    std::vector<double> time_values;

public:
    File();
    /// Open/create an ExodusII file
    ///
    /// @param file_path Path to the file to open/create
    /// @param file_access Desired file access
    ///  - ``exodusIIcpp::FileAccess::READ`` for reading,
    ///  - ``exodusIIcpp::FileAccess::WRITE`` for writing,
    ///  - ``exodusIIcpp::FileAccess::APPEND`` for appending to an existing file.
    explicit File(fs::path file_path, exodusIIcpp::FileAccess file_access);
    ~File();

    /// Open an ExodusII file
    ///
    /// @param file_path Path to the file to open
    void open(const fs::path & file_path);

    /// Create an ExodusII file
    ///
    /// @param file_path Path to the file to create
    void create(const fs::path & file_path);

    /// Open an existing ExodusII file for appending new time steps
    ///
    /// @param file_path Path to the file to open
    void append(const fs::path & file_path);

    /// Is file opened
    ///
    /// @return `true` if opened, `false` otherwise
    bool is_opened() const;

    /// Init the file before doing any *read* operations
    void init();

    /// Init the file before doing any *write* operations
    ///
    /// @param title Title of the file. Can be empty.
    /// @param n_dims Number of spatial dimensions. ``[1, 2, 3]``
    /// @param n_nodes Total number of nodes
    /// @param n_elems Total number of elements
    /// @param n_elem_blks Number of element blocks
    /// @param n_node_sets Number of node sets
    /// @param n_side_sets Number of side sets
    void init(const char * title,
              int n_dims,
              int n_nodes,
              int n_elems,
              int n_elem_blks,
              int n_node_sets,
              int n_side_sets);

    /// Get the title of the file
    ///
    /// @return The file title
    const std::string & get_title() const;

    /// Get the spatial dimension
    ///
    /// @return The spatial dimension
    int get_dim() const;

    /// Get the total number of nodes
    ///
    /// @return The total number of nodes
    int get_num_nodes() const;

    /// Get the total number of elements
    ///
    /// @return The total number of elements
    int get_num_elements() const;

    /// Get the number of element blocks
    ///
    /// @return The number of element blocks
    int get_num_element_blocks() const;

    /// Get the number of node sets
    ///
    /// @return The number of node sets
    int get_num_node_sets() const;

    /// Get the number of side sets
    ///
    /// @return The number of side sets
    int get_num_side_sets() const;

    /// Get the x-coordinates of nodes
    ///
    /// @return The x-coordinates of nodes
    const std::vector<double> & get_x_coords() const;

    /// Get the y-coordinates of nodes
    ///
    /// @return The y-coordinates of nodes
    const std::vector<double> & get_y_coords() const;

    /// Get the z-coordinates of nodes
    ///
    /// @return The z-coordinates of nodes
    const std::vector<double> & get_z_coords() const;

    /// Get coordinate names
    ///
    /// @return Array with coordinate names
    const std::vector<std::string> & get_coord_names() const;

    /// Get element blocks
    ///
    /// @return The list of element blocks
    const std::vector<ElementBlock> & get_element_blocks() const;

    /// Get an element block
    ///
    /// @param idx Index of the element block: `[0..<number of element blocks>)`
    /// @return The element block at index `idx`
    const ElementBlock & get_element_block(std::size_t idx) const;

    /// Get side sets
    ///
    /// @return The list of side sets
    const std::vector<SideSet> & get_side_sets() const;

    /// Get node list for a given side set index
    ///
    /// Example: For a side set that would have one EDGE2 and one TRI3, we would receive:
    /// - node_count_list: [2, 3]
    /// - node_list: [1, 2, 7, 2, 8]
    ///
    /// @param side_set_id Side set ID (not index)
    /// @param node_count_list Number of elements in the side set
    /// @param node_list Nodes corresponding to sides
    void get_side_set_node_list(int side_set_idx,
                                std::vector<int> & node_count_list,
                                std::vector<int> & node_list) const;

    /// Get node sets
    ///
    /// @return The list of node sets
    const std::vector<NodeSet> & get_node_sets() const;

    /// Get number of time steps
    ///
    /// @return Number of time steps
    int get_num_times() const;

    /// Get time values
    ///
    /// @return Time values
    const std::vector<double> & get_times() const;

    /// Get nodal variable names
    ///
    /// @return Nodal variable names
    std::vector<std::string> get_nodal_variable_names() const;

    /// Get elemental variable names
    ///
    /// @return Elemental variable names
    std::vector<std::string> get_elemental_variable_names() const;

    /// Get global variable names
    ///
    /// @return Global variable names
    std::vector<std::string> get_global_variable_names() const;

    /// Get nodal variable values at once
    ///
    /// @param time_step Time step index (1-based)
    /// @param var_idx Variable index (1-based)
    /// @return Vector of nodal values for the given variable
    std::vector<double> get_nodal_variable_values(int time_step, int var_idx) const;

    /// Get elemental variable values for a given block at once
    ///
    /// @param time_step Time step index (1-based)
    /// @param var_idx Variable index (1-based)
    /// @param block_id Block ID
    /// @return Vector of elemental values for a given variable and block
    std::vector<double>
    get_elemental_variable_values(int time_step, int var_idx, int block_id) const;

    /// Get values of global variables for a given time steps
    ///
    /// @param time_step Time step index (1-based)
    /// @return Values of global variables
    std::vector<double> get_global_variable_values(int time_step) const;

    /// Get global variable values over time
    ///
    /// @param var_idx Variable index (1-based)
    /// @param begin_idx First time step (1-based)
    /// @param end_idx End time step (1-based). -1 means use last time step.
    /// @return Values of a given global variable over specified time interval
    std::vector<double>
    get_global_variable_values(int var_idx, int begin_idx, int end_idx = -1) const;

    // Read API

    /// Read *all* data from the ExodusII file
    void read();

    /// Read coordinate data from the ExodusII file
    void read_coords();

    /// Read coordinate names from the ExodusII file
    void read_coord_names();

    /// Read element map from the ExodusII file
    void read_elem_map();

    /// Read element blocks from the ExodusII file
    void read_blocks();

    /// Read block names
    ///
    /// @return Map of block ID -> block name
    std::map<int, std::string> read_block_names() const;

    /// Read node sets from the ExodusII file
    void read_node_sets();

    /// Read node set names
    ///
    /// @return Map of Node set ID -> node set name
    std::map<int, std::string> read_node_set_names() const;

    /// Read side sets from the ExodusII file
    void read_side_sets();

    /// Read side set names
    ///
    /// @return Map of Side set ID -> side set name
    std::map<int, std::string> read_side_set_names() const;

    /// Read times
    void read_times();

    // Write API

    /// Write 1-D coordinates to the ExodusII file
    ///
    /// @param x x-coordinates
    void write_coords(const std::vector<double> & x);

    /// Write 2-D coordinates to the ExodusII file
    ///
    /// @param x x-coordinates
    /// @param y y-coordinates
    void write_coords(const std::vector<double> & x, const std::vector<double> & y);

    /// Write 3-D coordinates to the ExodusII file
    ///
    /// @param x x-coordinates
    /// @param y y-coordinates
    /// @param z z-coordinates
    void write_coords(const std::vector<double> & x,
                      const std::vector<double> & y,
                      const std::vector<double> & z);

    /// Write coordinate names to the ExodusII file
    void write_coord_names();

    /// Write custom coordinate names to the ExodusII file
    ///
    /// @param coord_names Array with coordinate names
    void write_coord_names(const std::vector<std::string> & coord_names);

    /// Write information records to the ExodusII file
    ///
    /// @param info List of information records
    void write_info(std::vector<std::string> info);

    /// Write time slice to the ExodusII file
    ///
    /// @param time_step Time step number
    /// @param time Time corresponding to the time step
    void write_time(int time_step, double time);

    /// Write node set names to the ExodusII file
    ///
    /// @param names Names of node sets
    void write_node_set_names(const std::vector<std::string> & names);

    /// Write node set to the ExodusII file
    ///
    /// @param set_id Node set index
    /// @param node_set List of node IDs that comprise the node set
    void write_node_set(int64_t set_id, const std::vector<int> & node_set);

    /// Write side set names to the ExodusII file
    ///
    /// @param names Names of side sets
    void write_side_set_names(const std::vector<std::string> & names);

    /// Write side set to the ExodusII file
    ///
    /// @param set_id Side set index
    /// @param elem_list List of element IDs that comprise the side set
    /// @param side_list List of local side numbers that comprise the side set
    void write_side_set(int64_t set_id,
                        const std::vector<int> & elem_list,
                        const std::vector<int> & side_list);

    /// Write element block names to the ExodusII file
    ///
    /// @param names Names of element blocks
    void write_block_names(const std::vector<std::string> & names);

    /// Write element block to the ExodusII file
    ///
    /// @param blk_id Element block index
    /// @param elem_type Element type
    /// @param n_elems_in_block Number of elements in the block
    /// @param connect Connectivity array ``[el1_n1, el1_n2, ..., el2_n1, el2_n2, ...]``
    /// @note The number of nodes per element is infered from the size of ``connect`` array and the
    /// number of elements in the block `n_elems_in_block`.
    void write_block(int64_t blk_id,
                     const char * elem_type,
                     int64_t n_elems_in_block,
                     const std::vector<int> & connect);

    /// Write nodal variable names to the ExodusII file
    ///
    /// @param var_names Names of nodal variables
    void write_nodal_var_names(const std::vector<std::string> & var_names);

    /// Write elemental variable names to the ExodusII file
    ///
    /// @param var_names Names of elemental variables
    void write_elem_var_names(const std::vector<std::string> & var_names);

    /// Write global variable names to the ExodusII file
    ///
    /// @param var_names Names of global variables
    void write_global_var_names(const std::vector<std::string> & var_names);

    /// Write nodal variable values to the ExodusII file
    ///
    /// @param step_num Time step index
    /// @param var_index Variable index
    /// @param values Values to write
    void write_nodal_var(int step_num, int var_index, const std::vector<double> & values);

    /// Write nodal variable value to the ExodusII file
    ///
    /// @param step_num Time step index
    /// @param var_index Variable index
    /// @param obj_id Object ID
    /// @param start_index Index of the value
    /// @param var_value Value to write
    void write_partial_nodal_var(int step_num,
                                 int var_index,
                                 int64_t obj_id,
                                 int64_t start_index,
                                 double var_value);

    /// Write elemental variable value to the ExodusII file
    ///
    /// @param step_num Time step index
    /// @param var_index Variable index
    /// @param obj_id Object ID
    /// @param start_index Index of the value
    /// @param var_value Value to write
    void write_partial_elem_var(int step_num,
                                int var_index,
                                int64_t obj_id,
                                int64_t start_index,
                                double var_value);

    /// Write global variable value to the ExodusII file
    ///
    /// @param step_num Time step index
    /// @param var_index Variable index
    /// @param value Value to write
    void write_global_var(int step_num, int var_index, double value);

    /// Update the file
    ///
    /// Call after the time step data were all saved
    void update();

    /// Close the file
    void close();
};

} // namespace exodusIIcpp
