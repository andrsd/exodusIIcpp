// SPDX-FileCopyrightText: 2025 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>
#include "exodusIIcpp/exodusIIcpp.h"
#include "version.h"

using namespace exodusIIcpp;

namespace py = pybind11;

PYBIND11_MODULE(exodusIIcpp, m)
{
    m.doc() = "pybind11 plugin for exodusIIcpp";
    py::setattr(m, "version", py::str(EXODUSIICPP_VERSION));

    py::enum_<exodusIIcpp::FileAccess>(m, "FileAccess")
        .value("READ", exodusIIcpp::FileAccess::READ)
        .value("WRITE", exodusIIcpp::FileAccess::WRITE)
        .value("APPEND", exodusIIcpp::FileAccess::APPEND);

    py::class_<exodusIIcpp::ElementBlock>(m, "ElementBlock")
        .def(py::init())
        .def("get_id", &ElementBlock::get_id)
        .def("get_name", &ElementBlock::get_name)
        .def("get_size", &ElementBlock::get_size)
        .def("get_num_nodes_per_element", &ElementBlock::get_num_nodes_per_element)
        .def("get_element_type", &ElementBlock::get_element_type)
        .def("get_element_connectivity", &ElementBlock::get_element_connectivity)
        .def("get_num_elements", &ElementBlock::get_num_elements)
        .def("get_connectivity", &ElementBlock::get_connectivity)
        .def("set_id", &ElementBlock::set_id)
        .def("set_name", &ElementBlock::set_name)
        .def("set_connectivity", &ElementBlock::set_connectivity);

    py::class_<exodusIIcpp::NodeSet>(m, "NodeSet")
        .def(py::init())
        .def("get_id", &NodeSet::get_id)
        .def("get_name", &NodeSet::get_name)
        .def("get_size", &NodeSet::get_size)
        .def("get_node_id", &NodeSet::get_node_id)
        .def("get_node_ids", &NodeSet::get_node_ids)
        .def("set_id", &NodeSet::set_id)
        .def("set_name", &NodeSet::set_name)
        .def("set_nodes", &NodeSet::set_nodes);

    py::class_<exodusIIcpp::SideSet>(m, "SideSet")
        .def(py::init())
        .def("get_id", &SideSet::get_id)
        .def("get_name", &SideSet::get_name)
        .def("get_size", &SideSet::get_size)
        .def("get_element_id", &SideSet::get_element_id)
        .def("get_element_ids", &SideSet::get_element_ids)
        .def("get_side_ids", &SideSet::get_side_ids)
        .def("get_side_id", &SideSet::get_side_id)
        .def("set_id", &SideSet::set_id)
        .def("set_name", &SideSet::set_name)
        .def("set_sides", &SideSet::set_sides)
        .def("add", &SideSet::add);

    py::class_<exodusIIcpp::File>(m, "File")
        .def(py::init())
        .def(py::init<const fs::path &, exodusIIcpp::FileAccess>())
        .def("open", &File::open)
        .def("create", &File::create)
        .def("append", &File::append)
        .def("is_opened", &File::is_opened)
        .def("init", static_cast<void (File::*)()>(&File::init))
        .def("init",
             static_cast<void (File::*)(const char *, int, int, int, int, int, int)>(&File::init))
        //
        .def("get_title", &File::get_title)
        .def("get_dim", &File::get_dim)
        .def("get_num_nodes", &File::get_num_nodes)
        .def("get_num_elements", &File::get_num_elements)
        .def("get_num_element_blocks", &File::get_num_element_blocks)
        .def("get_num_node_sets", &File::get_num_node_sets)
        .def("get_num_side_sets", &File::get_num_side_sets)
        .def("get_x_coords", &File::get_x_coords)
        .def("get_y_coords", &File::get_y_coords)
        .def("get_z_coords", &File::get_z_coords)
        .def("get_coord_names", &File::get_coord_names)
        .def("get_element_block", &File::get_element_block)
        .def("get_element_blocks", &File::get_element_blocks)
        .def("get_side_sets", &File::get_side_sets)
        .def("get_side_set_node_list",
             [](const File & self, int side_set_idx) {
                 std::vector<int> node_count_list, node_list;
                 self.get_side_set_node_list(side_set_idx, node_count_list, node_list);
                 return py::make_tuple(node_count_list, node_list);
             })
        .def("get_node_sets", &File::get_node_sets)
        .def("get_num_times", &File::get_num_times)
        .def("get_times", &File::get_times)
        .def("get_nodal_variable_names", &File::get_nodal_variable_names)
        .def("get_elemental_variable_names", &File::get_elemental_variable_names)
        .def("get_global_variable_names", &File::get_global_variable_names)
        .def("get_nodal_variable_values", &File::get_nodal_variable_values)
        .def("get_elemental_variable_values", &File::get_elemental_variable_values)
        .def("get_global_variable_values",
             static_cast<std::vector<double> (File::*)(int) const>(
                 &File::get_global_variable_values))
        .def("get_global_variable_values",
             static_cast<std::vector<double> (File::*)(int, int, int) const>(
                 &File::get_global_variable_values),
             py::arg("var_idx"),
             py::arg("begin_idx"),
             py::arg("end_idx") = -1)
        // read
        .def("read", &File::read)
        .def("read_coords", &File::read_coords)
        .def("read_coord_names", &File::read_coord_names)
        .def("read_elem_map", &File::read_elem_map)
        .def("read_blocks", &File::read_blocks)
        .def("read_block_names", &File::read_block_names)
        .def("read_node_sets", &File::read_node_sets)
        .def("read_node_set_names", &File::read_node_set_names)
        .def("read_side_sets", &File::read_side_sets)
        .def("read_side_set_names", &File::read_side_set_names)
        .def("read_times", &File::read_times)
        // write
        .def("write_coords",
             static_cast<void (File::*)(const std::vector<double> &)>(&File::write_coords))
        .def("write_coords",
             static_cast<void (File::*)(const std::vector<double> &, const std::vector<double> &)>(
                 &File::write_coords))
        .def("write_coords",
             static_cast<void (File::*)(const std::vector<double> &,
                                        const std::vector<double> &,
                                        const std::vector<double> &)>(&File::write_coords))
        .def("write_coord_names", static_cast<void (File::*)()>(&File::write_coord_names))
        .def(
            "write_coord_names",
            static_cast<void (File::*)(const std::vector<std::string> &)>(&File::write_coord_names))
        .def("write_info", &File::write_info)
        .def("write_time", &File::write_time)
        .def("write_node_set_names", &File::write_node_set_names)
        .def("write_node_set", &File::write_node_set)
        .def("write_side_set_names", &File::write_side_set_names)
        .def("write_side_set", &File::write_side_set)
        .def("write_block_names", &File::write_block_names)
        .def("write_block", &File::write_block)
        .def("write_nodal_var_names", &File::write_nodal_var_names)
        .def("write_elem_var_names", &File::write_elem_var_names)
        .def("write_global_var_names", &File::write_global_var_names)
        .def("write_nodal_var", &File::write_nodal_var)
        .def("write_partial_nodal_var", &File::write_partial_nodal_var)
        .def("write_partial_elem_var", &File::write_partial_elem_var)
        .def("write_global_var", &File::write_global_var)
        //
        .def("update", &File::update)
        .def("close", &File::close);
}
