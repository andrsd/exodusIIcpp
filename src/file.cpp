// SPDX-FileCopyrightText: 2022 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#include "exodusIIcpp/file.h"
#include "exodusII.h"
#include "fmt/printf.h"
#include <cassert>
#include <numeric>

namespace exodusIIcpp {

static void
write_variable_names(int exoid, ex_entity_type obj_type, const std::vector<std::string> & var_names)
{
    int n_vars = var_names.size();
    if (n_vars == 0)
        return;

    EXODUSIICPP_CHECK_ERROR(ex_put_variable_param(exoid, obj_type, n_vars));
    std::vector<const char *> names(n_vars);
    for (int i = 0; i < n_vars; i++)
        names[i] = var_names[i].c_str();
    EXODUSIICPP_CHECK_ERROR(ex_put_variable_names(exoid, obj_type, n_vars, (char **) names.data()));
}

static void
write_names(int exoid, ex_entity_type obj_type, const std::vector<std::string> & names)
{
    int n = names.size();
    if (n == 0)
        return;

    std::vector<const char *> c_names(n);
    for (int i = 0; i < n; i++)
        c_names[i] = names[i].c_str();
    EXODUSIICPP_CHECK_ERROR(ex_put_names(exoid, obj_type, (char **) c_names.data()));
}

static std::map<int, std::string>
read_name_map(int exoid, int n, ex_entity_type obj_type)
{
    std::map<int, std::string> map_names;
    int * ids = new int[n];
    ex_get_ids(exoid, obj_type, ids);
    char name[MAX_STR_LENGTH + 1];
    for (int i = 0; i < n; i++) {
        ex_get_name(exoid, obj_type, ids[i], name);
        std::string str_name;
        if (strnlen(name, MAX_STR_LENGTH) > 0)
            str_name = name;
        map_names[ids[i]] = str_name;
    }
    delete[] ids;
    return map_names;
}

static std::vector<std::string>
read_variable_names(int exoid, ex_entity_type obj_type)
{
    int n_vars;
    EXODUSIICPP_CHECK_ERROR(ex_get_variable_param(exoid, obj_type, &n_vars));
    std::vector<std::string> var_names;
    if (n_vars > 0) {
        var_names.resize(n_vars);
        for (int i = 0; i < n_vars; i++)
            var_names[i].resize(MAX_STR_LENGTH);
        std::vector<char *> names(n_vars);
        for (int i = 0; i < n_vars; i++)
            names[i] = var_names[i].data();
        EXODUSIICPP_CHECK_ERROR(ex_get_variable_names(exoid, obj_type, n_vars, names.data()));
        for (int i = 0; i < n_vars; i++)
            var_names[i] = var_names[i].c_str();
    }
    return var_names;
}

File::File() :
    cpu_word_size(sizeof(double)),
    io_word_size(8),
    version(0),
    exoid(-1),
    n_dim(-1),
    n_nodes(-1),
    n_elems(-1),
    n_elem_blks(-1),
    n_node_sets(-1),
    n_side_sets(-1)
{
}

File::File(exodusIIcpp::fs::path file_path, exodusIIcpp::FileAccess file_access) :
    file_access(file_access),
    cpu_word_size(sizeof(double)),
    io_word_size(8),
    version(0),
    exoid(-1),
    n_dim(-1),
    n_nodes(-1),
    n_elems(-1),
    n_elem_blks(-1),
    n_node_sets(-1),
    n_side_sets(-1)
{
    if (file_access == exodusIIcpp::FileAccess::READ) {
        open(file_path.string());
        init();
    }
    else
        create(file_path.string());
}

File::~File()
{
    close();
}

void
File::open(const std::string & file_path)
{
    this->file_access = exodusIIcpp::FileAccess::READ;
    this->exoid = ex_open(file_path.c_str(),
                          EX_READ,
                          &this->cpu_word_size,
                          &this->io_word_size,
                          &this->version);
    if (this->exoid < 0)
        throw Exception(fmt::sprintf("Unable to open file '%s'.", file_path));
}

void
File::create(const std::string & file_path)
{
    this->file_access = exodusIIcpp::FileAccess::WRITE;
    this->exoid =
        ex_create(file_path.c_str(), EX_CLOBBER, &this->cpu_word_size, &this->io_word_size);
    if (this->exoid < 0)
        throw Exception(fmt::sprintf("Unable to open file '%s'.", file_path));
}

bool
File::is_opened() const
{
    return this->exoid != -1;
}

void
File::init()
{
    if (this->file_access == exodusIIcpp::FileAccess::READ) {
        char title[MAX_LINE_LENGTH + 1];
        memset(title, 0, sizeof(title));
        EXODUSIICPP_CHECK_ERROR(ex_get_init(this->exoid,
                                            title,
                                            &this->n_dim,
                                            &this->n_nodes,
                                            &this->n_elems,
                                            &this->n_elem_blks,
                                            &this->n_node_sets,
                                            &this->n_side_sets));
        this->title = title;
        this->coord_names.resize(this->n_dim);
    }
    else
        throw Exception("Calling init with non-read file access.");
}

void
File::init(const char * title,
           int n_dims,
           int n_nodes,
           int n_elems,
           int n_elem_blks,
           int n_node_sets,
           int n_side_sets)
{
    if (this->file_access == exodusIIcpp::FileAccess::WRITE) {
        EXODUSIICPP_CHECK_ERROR(ex_put_init(this->exoid,
                                            title,
                                            n_dims,
                                            n_nodes,
                                            n_elems,
                                            n_elem_blks,
                                            n_node_sets,
                                            n_side_sets));
        this->coord_names.resize(n_dims);
    }
    else
        throw Exception("Calling init with non-write access.");
}

const std::string &
File::get_title() const
{
    return this->title;
}

int
File::get_dim() const
{
    return this->n_dim;
}

int
File::get_num_nodes() const
{
    return this->n_nodes;
}

int
File::get_num_elements() const
{
    return this->n_elems;
}

int
File::get_num_element_blocks() const
{
    return this->n_elem_blks;
}

int
File::get_num_node_sets() const
{
    return this->n_node_sets;
}

int
File::get_num_side_sets() const
{
    return this->n_side_sets;
}

const std::vector<double> &
File::get_x_coords() const
{
    return this->x;
}

const std::vector<double> &
File::get_y_coords() const
{
    return this->y;
}

const std::vector<double> &
File::get_z_coords() const
{
    return this->z;
}

const std::vector<std::string> &
File::get_coord_names() const
{
    return this->coord_names;
}

const std::vector<ElementBlock> &
File::get_element_blocks() const
{
    return this->element_blocks;
}

const ElementBlock &
File::get_element_block(std::size_t idx) const
{
    if (idx < this->element_blocks.size())
        return this->element_blocks[idx];
    else
        throw Exception(fmt::sprintf("Index out of range '%d'", idx));
}

const std::vector<SideSet> &
File::get_side_sets() const
{
    return this->side_sets;
}

void
File::get_side_set_node_list(int side_set_id,
                             std::vector<int> & node_count_list,
                             std::vector<int> & node_list) const
{
    int num_sides_in_set;
    EXODUSIICPP_CHECK_ERROR(
        ex_get_set_param(this->exoid, EX_SIDE_SET, side_set_id, &num_sides_in_set, nullptr));

    node_count_list.resize(num_sides_in_set);
    // the maximum number of nodes of a 2D element (which would be a side element) is 9 on QUAD9
    node_list.resize(num_sides_in_set * 9);
    EXODUSIICPP_CHECK_ERROR(ex_get_side_set_node_list(this->exoid,
                                                      side_set_id,
                                                      node_count_list.data(),
                                                      node_list.data()));
    int n = std::accumulate(node_count_list.begin(), node_count_list.end(), 0);
    node_list.resize(n);
}

const std::vector<NodeSet> &
File::get_node_sets() const
{
    return this->node_sets;
}

int
File::get_num_times() const
{
    return ex_inquire_int(this->exoid, EX_INQ_TIME);
}

const std::vector<double> &
File::get_times() const
{
    return this->time_values;
}

std::vector<std::string>
File::get_nodal_variable_names() const
{
    return read_variable_names(this->exoid, EX_NODAL);
}

std::vector<std::string>
File::get_elemental_variable_names() const
{
    return read_variable_names(this->exoid, EX_ELEM_BLOCK);
}

std::vector<std::string>
File::get_global_variable_names() const
{
    return read_variable_names(this->exoid, EX_GLOBAL);
}

std::vector<double>
File::get_nodal_variable_values(int time_step, int var_idx) const
{
    std::vector<double> values;
    values.resize(this->n_nodes);
    EXODUSIICPP_CHECK_ERROR(
        ex_get_var(this->exoid, time_step, EX_NODAL, var_idx, 1, this->n_nodes, values.data()));
    return values;
}

std::vector<double>
File::get_elemental_variable_values(int time_step, int var_idx, int block_id) const
{
    std::vector<double> values;
    int n_blk_elems;
    EXODUSIICPP_CHECK_ERROR(ex_get_block(this->exoid,
                                         EX_ELEM_BLOCK,
                                         block_id,
                                         nullptr,
                                         &n_blk_elems,
                                         nullptr,
                                         nullptr,
                                         nullptr,
                                         nullptr));
    values.resize(n_blk_elems);
    EXODUSIICPP_CHECK_ERROR(ex_get_var(this->exoid,
                                       time_step,
                                       EX_ELEM_BLOCK,
                                       var_idx,
                                       block_id,
                                       n_blk_elems,
                                       values.data()));
    return values;
}

std::vector<double>
File::get_global_variable_values(int time_step) const
{
    int n_glob_vars;
    EXODUSIICPP_CHECK_ERROR(ex_get_variable_param(this->exoid, EX_GLOBAL, &n_glob_vars));

    std::vector<double> values(n_glob_vars);
    EXODUSIICPP_CHECK_ERROR(
        ex_get_var(this->exoid, time_step, EX_GLOBAL, 1, 0, n_glob_vars, values.data()));
    return values;
}

std::vector<double>
File::get_global_variable_values(int var_idx, int begin_idx, int end_idx) const
{
    int last_idx = end_idx == -1 ? get_num_times() : end_idx;
    int n_vals = last_idx - begin_idx + 1;
    std::vector<double> values(n_vals);
    EXODUSIICPP_CHECK_ERROR(
        ex_get_var_time(this->exoid, EX_GLOBAL, var_idx, 1, begin_idx, last_idx, values.data()));
    return values;
}

// Read API

void
File::read()
{
    read_coords();
    read_coord_names();
    read_blocks();
    read_node_sets();
    read_side_sets();
    read_times();
}

void
File::read_coords()
{
    this->x.clear();
    this->y.clear();
    this->z.clear();

    this->x.resize(this->n_nodes);
    EXODUSIICPP_CHECK_ERROR(ex_get_coord(this->exoid, this->x.data(), nullptr, nullptr));

    if (this->n_dim >= 2) {
        this->y.resize(this->n_nodes);
        EXODUSIICPP_CHECK_ERROR(ex_get_coord(this->exoid, nullptr, this->y.data(), nullptr));
    }

    if (this->n_dim == 3) {
        this->z.resize(this->n_nodes);
        EXODUSIICPP_CHECK_ERROR(ex_get_coord(this->exoid, nullptr, nullptr, this->z.data()));
    }
}

void
File::read_coord_names()
{
    char * names[3] = { nullptr, nullptr, nullptr };
    for (unsigned int i = 0; i < this->n_dim; i++) {
        names[i] = (char *) calloc((MAX_STR_LENGTH + 1), sizeof(char));
        memset(names[i], 0, (MAX_STR_LENGTH + 1) * sizeof(char));
    }

    EXODUSIICPP_CHECK_ERROR(ex_get_coord_names(this->exoid, names));
    for (unsigned int i = 0; i < this->n_dim; i++) {
        this->coord_names[i].assign(names[i]);
        free(names[i]);
    }
}

void
File::read_elem_map()
{
    this->elem_map.clear();
    this->elem_map.resize(this->n_elems);
    EXODUSIICPP_CHECK_ERROR(ex_get_map(this->exoid, this->elem_map.data()));
}

void
File::read_blocks()
{
    if (this->n_elem_blks <= 0)
        return;

    std::vector<int> block_ids(this->n_elem_blks);
    EXODUSIICPP_CHECK_ERROR(ex_get_ids(this->exoid, EX_ELEM_BLOCK, block_ids.data()));

    for (auto & id : block_ids) {
        char name[MAX_STR_LENGTH + 1];
        EXODUSIICPP_CHECK_ERROR(ex_get_name(this->exoid, EX_ELEM_BLOCK, id, name));

        char elem_type[MAX_STR_LENGTH + 1];
        int n_elems_in_block;
        int n_nodes_per_elem;
        int n_attrs;
        EXODUSIICPP_CHECK_ERROR(ex_get_block(this->exoid,
                                             EX_ELEM_BLOCK,
                                             id,
                                             elem_type,
                                             &n_elems_in_block,
                                             &n_nodes_per_elem,
                                             nullptr,
                                             nullptr,
                                             &n_attrs));

        ElementBlock eb;
        eb.set_id(id);
        eb.set_name(name);
        if (n_elems_in_block > 0) {
            std::vector<int> connect((std::size_t) n_elems_in_block * n_nodes_per_elem);
            EXODUSIICPP_CHECK_ERROR(
                ex_get_conn(this->exoid, EX_ELEM_BLOCK, id, connect.data(), 0, 0));
            eb.set_connectivity(elem_type, n_elems_in_block, n_nodes_per_elem, connect);
        }

        this->element_blocks.push_back(eb);
    }
}

std::map<int, std::string>
File::read_block_names() const
{
    return read_name_map(this->exoid, this->n_elem_blks, EX_ELEM_BLOCK);
}

void
File::read_node_sets()
{
    if (this->n_node_sets <= 0)
        return;

    std::vector<int> node_set_ids(this->n_node_sets);
    EXODUSIICPP_CHECK_ERROR(ex_get_ids(this->exoid, EX_NODE_SET, node_set_ids.data()));

    for (auto & id : node_set_ids) {
        char name[MAX_STR_LENGTH + 1];
        EXODUSIICPP_CHECK_ERROR(ex_get_name(this->exoid, EX_NODE_SET, id, name));

        int n_nodes;
        int n_dfs;
        EXODUSIICPP_CHECK_ERROR(ex_get_set_param(this->exoid, EX_NODE_SET, id, &n_nodes, &n_dfs));

        std::vector<int> node_list(n_nodes);
        EXODUSIICPP_CHECK_ERROR(ex_get_set(this->exoid, EX_NODE_SET, id, node_list.data(), 0));

        NodeSet ns;
        ns.set_id(id);
        ns.set_name(name);
        ns.set_nodes(node_list);

        this->node_sets.push_back(ns);
    }
}

std::map<int, std::string>
File::read_node_set_names() const
{
    return read_name_map(this->exoid, this->n_node_sets, EX_NODE_SET);
}

void
File::read_side_sets()
{
    if (this->n_side_sets <= 0)
        return;

    std::vector<int> side_set_ids(this->n_side_sets);
    EXODUSIICPP_CHECK_ERROR(ex_get_ids(this->exoid, EX_SIDE_SET, side_set_ids.data()));

    for (auto & id : side_set_ids) {
        char name[MAX_STR_LENGTH + 1];
        EXODUSIICPP_CHECK_ERROR(ex_get_name(this->exoid, EX_SIDE_SET, id, name));

        int n_sides;
        int n_dfs;
        EXODUSIICPP_CHECK_ERROR(ex_get_set_param(this->exoid, EX_SIDE_SET, id, &n_sides, &n_dfs));

        std::vector<int> elem_list(n_sides);
        std::vector<int> side_list(n_sides);
        EXODUSIICPP_CHECK_ERROR(
            ex_get_set(this->exoid, EX_SIDE_SET, id, elem_list.data(), side_list.data()));

        SideSet ss;
        ss.set_id(id);
        ss.set_name(name);
        ss.set_sides(elem_list, side_list);

        this->side_sets.push_back(ss);
    }
}

std::map<int, std::string>
File::read_side_set_names() const
{
    return read_name_map(this->exoid, this->n_side_sets, EX_SIDE_SET);
}

void
File::read_times()
{
    this->time_values.resize(get_num_times());
    EXODUSIICPP_CHECK_ERROR(ex_get_all_times(this->exoid, this->time_values.data()));
}

// Write API

void
File::write_coords(const std::vector<double> & x)
{
    EXODUSIICPP_CHECK_ERROR(ex_put_coord(this->exoid, x.data(), nullptr, nullptr));
}

void
File::write_coords(const std::vector<double> & x, const std::vector<double> & y)
{
    EXODUSIICPP_CHECK_ERROR(ex_put_coord(this->exoid, x.data(), y.data(), nullptr));
}

void
File::write_coords(const std::vector<double> & x,
                   const std::vector<double> & y,
                   const std::vector<double> & z)
{
    EXODUSIICPP_CHECK_ERROR(ex_put_coord(this->exoid, x.data(), y.data(), z.data()));
}

void
File::write_coord_names()
{
    write_coord_names({ "x", "y", "z" });
}

void
File::write_coord_names(const std::vector<std::string> & new_names)
{
    assert(new_names.size() <= 3);
    const char * names[3] = { nullptr, nullptr, nullptr };
    for (unsigned int i = 0; i < new_names.size(); i++)
        names[i] = new_names[i].c_str();
    EXODUSIICPP_CHECK_ERROR(ex_put_coord_names(this->exoid, (char **) names));
    this->coord_names = new_names;
}

void
File::write_info(std::vector<std::string> info)
{
    for (auto & s : info)
        s.resize(MAX_LINE_LENGTH);
    std::vector<const char *> arr(info.size());
    for (std::size_t i = 0; i < info.size(); i++)
        arr[i] = info[i].c_str();
    EXODUSIICPP_CHECK_ERROR(ex_put_info(this->exoid, (int) info.size(), (char **) arr.data()));
}

void
File::write_time(int time_step, double time)
{
    EXODUSIICPP_CHECK_ERROR(ex_put_time(this->exoid, time_step, &time));
}

void
File::write_node_set_names(const std::vector<std::string> & names)
{
    write_names(this->exoid, EX_NODE_SET, names);
}

void
File::write_node_set(int64_t set_id, const std::vector<int> & node_set)
{
    EXODUSIICPP_CHECK_ERROR(
        ex_put_set_param(this->exoid, EX_NODE_SET, set_id, (int64_t) node_set.size(), 0));
    EXODUSIICPP_CHECK_ERROR(ex_put_set(this->exoid, EX_NODE_SET, set_id, node_set.data(), nullptr));
}

void
File::write_side_set_names(const std::vector<std::string> & names)
{
    write_names(this->exoid, EX_SIDE_SET, names);
}

void
File::write_side_set(int64_t set_id,
                     const std::vector<int> & elem_list,
                     const std::vector<int> & side_list)
{
    if (elem_list.size() == side_list.size()) {
        EXODUSIICPP_CHECK_ERROR(
            ex_put_set_param(this->exoid, EX_SIDE_SET, set_id, (int64_t) elem_list.size(), 0));
        EXODUSIICPP_CHECK_ERROR(
            ex_put_set(this->exoid, EX_SIDE_SET, set_id, elem_list.data(), side_list.data()));
    }
    else
        throw Exception("The length of elem_list must be equal to the length of side_list.");
}

void
File::write_block_names(const std::vector<std::string> & names)
{
    write_names(this->exoid, EX_ELEM_BLOCK, names);
}

void
File::write_block(int64_t blk_id,
                  const char * elem_type,
                  int64_t n_elems_in_block,
                  const std::vector<int> & connect)
{
    EXODUSIICPP_CHECK_ERROR(ex_put_block(this->exoid,
                                         EX_ELEM_BLOCK,
                                         blk_id,
                                         elem_type,
                                         n_elems_in_block,
                                         connect.size() / n_elems_in_block,
                                         0,
                                         0,
                                         0));
    EXODUSIICPP_CHECK_ERROR(
        ex_put_conn(this->exoid, EX_ELEM_BLOCK, blk_id, connect.data(), nullptr, nullptr));
}

void
File::write_nodal_var_names(const std::vector<std::string> & var_names)
{
    write_variable_names(this->exoid, EX_NODAL, var_names);
}

void
File::write_elem_var_names(const std::vector<std::string> & var_names)
{
    write_variable_names(this->exoid, EX_ELEM_BLOCK, var_names);
}

void
File::write_global_var_names(const std::vector<std::string> & var_names)
{
    write_variable_names(this->exoid, EX_GLOBAL, var_names);
}

void
File::write_nodal_var(int step_num, int var_index, const std::vector<double> & values)
{
    EXODUSIICPP_CHECK_ERROR(
        ex_put_var(this->exoid, step_num, EX_NODAL, var_index, 0, values.size(), values.data()));
}

void
File::write_partial_nodal_var(int step_num,
                              int var_index,
                              int64_t obj_id,
                              int64_t start_index,
                              double var_value)
{
    EXODUSIICPP_CHECK_ERROR(ex_put_partial_var(this->exoid,
                                               step_num,
                                               EX_NODAL,
                                               var_index,
                                               obj_id,
                                               start_index,
                                               1,
                                               &var_value));
}

void
File::write_partial_elem_var(int step_num,
                             int var_index,
                             int64_t obj_id,
                             int64_t start_index,
                             double var_value)
{
    EXODUSIICPP_CHECK_ERROR(ex_put_partial_var(this->exoid,
                                               step_num,
                                               EX_ELEM_BLOCK,
                                               var_index,
                                               obj_id,
                                               start_index,
                                               1,
                                               &var_value));
}

void
File::write_global_var(int step_num, int var_index, double value)
{
    EXODUSIICPP_CHECK_ERROR(ex_put_var(this->exoid, step_num, EX_GLOBAL, var_index, 0, 1, &value));
}

void
File::update()
{
    EXODUSIICPP_CHECK_ERROR(ex_update(this->exoid));
}

void
File::close()
{
    if (is_opened()) {
        EXODUSIICPP_CHECK_ERROR(ex_close(this->exoid));
        this->exoid = -1;
    }
}

} // namespace exodusIIcpp
