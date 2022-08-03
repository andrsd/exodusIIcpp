#pragma once

#include <vector>
#include "exodusII.h"
#include "exodusIIcppElementBlock.h"
#include "exodusIIcppEnums.h"
#include "exodusIIcppError.h"
#include "exodusIIcppNodeSet.h"
#include "exodusIIcppSideSet.h"
#include "exodusIIcppFilesystem.h"

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
    ///
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
    /// Element map
    std::vector<int> elem_map;
    /// Element blocks
    std::vector<ElementBlock> element_blocks;
    /// Face sets
    std::vector<SideSet> side_sets;
    /// Node sets
    std::vector<NodeSet> node_sets;

public:
    File();
    explicit File(exodusIIcpp::fs::path file_path, exodusIIcpp::FileAccess file_access);
    ~File();

    /// Open file
    ///
    /// @param file_path File path to the file to
    void open(const std::string & file_path);

    void create(const std::string & file_path);

    bool is_opened() const;

    void init();

    void init(const char * title,
              int n_dims,
              int n_nodes,
              int n_elems,
              int n_elem_blks,
              int n_node_sets,
              int n_side_sets);

    int get_dim() const;

    int get_num_nodes() const;

    int get_num_elements() const;

    int get_num_element_blocks() const;

    int get_num_node_sets() const;

    int get_num_side_sets() const;

    const std::vector<double> & get_x_coords() const;

    const std::vector<double> & get_y_coords() const;

    const std::vector<double> & get_z_coords() const;

    const std::vector<ElementBlock> & get_element_blocks() const;

    const ElementBlock & get_element_block(std::size_t idx) const;

    const std::vector<SideSet> & get_side_sets() const;

    const std::vector<NodeSet> & get_node_sets() const;

    // Read API

    void read();

    void read_coords();

    void read_elem_map();

    void read_blocks();

    void read_node_sets();

    void read_side_sets();

    // Write API

    void write_coords(const std::vector<double> & x);

    void write_coords(const std::vector<double> & x, const std::vector<double> & y);

    void write_coords(const std::vector<double> & x,
                      const std::vector<double> & y,
                      const std::vector<double> & z);

    void write_coord_names();

    void write_info(std::vector<std::string> info);

    void write_time(int time_step, double time);

    void write_node_set_names(const std::vector<std::string> & names);

    void write_node_set(ex_entity_id set_id, const std::vector<int> & node_set);

    void write_side_set_names(const std::vector<std::string> & names);

    void write_side_set(ex_entity_id set_id,
                        const std::vector<int> & elem_list,
                        const std::vector<int> & side_list);

    void write_block_names(const std::vector<std::string> & names);

    void write_block(ex_entity_id blk_id,
                     const char * elem_type,
                     int64_t n_elems_in_block,
                     const std::vector<int> & connect);

    void write_nodal_var_names(const std::vector<std::string> & var_names);

    void write_elem_var_names(const std::vector<std::string> & var_names);

    void write_global_var_names(const std::vector<std::string> & var_names);

    void write_partial_nodal_var(int step_num,
                                 int var_index,
                                 ex_entity_id obj_id,
                                 int64_t start_index,
                                 double var_value);

    void write_partial_elem_var(int step_num,
                                int var_index,
                                ex_entity_id obj_id,
                                int64_t start_index,
                                double var_value);

    void write_global_var(int step_num, int var_index, double value);

    void update();

    void close();

protected:
    void write_variable_names(ex_entity_type obj_type, const std::vector<std::string> & var_names);

    void write_names(ex_entity_type obj_type, const std::vector<std::string> & names);
};

} // namespace exodusIIcpp
