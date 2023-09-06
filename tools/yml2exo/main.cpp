#include "cxxopts/cxxopts.hpp"
#include "fmt/printf.h"
#include "yaml-cpp/yaml.h"
#include "exodusIIcpp.h"
#include "common/error.h"

YAML::Node
load_yaml(const std::string & file_name)
{
    return YAML::LoadFile(file_name);
}

void
write_coordinate_names(exodusIIcpp::File & exo, int dim, const YAML::Node & yml_coords)
{
    if (yml_coords) {
        auto names = yml_coords.as<std::vector<std::string>>();
        exo.write_coord_names(names);
    }
    else {
        if (dim == 1)
            exo.write_coord_names({ "x" });
        else if (dim == 2)
            exo.write_coord_names({ "x", "y" });
        else if (dim == 3)
            exo.write_coord_names({ "x", "y", "z" });
        else
            error("Unsupported dimension '{}'.", dim);
    }
}

void
write_coordinates(exodusIIcpp::File & exo, int dim, const YAML::Node & yml_coords)
{
    std::vector<double> xyz[3];

    for (auto & node : yml_coords) {
        auto coord = node.as<std::vector<double>>();
        if (coord.size() != dim)
            error("Mismatch in mesh dimension and coordinate dimension.");
        for (std::size_t i = 0; i < coord.size(); i++)
            xyz[i].push_back(coord[i]);
    }

    if (dim == 1)
        exo.write_coords(xyz[0]);
    else if (dim == 2)
        exo.write_coords(xyz[0], xyz[1]);
    else if (dim == 3)
        exo.write_coords(xyz[0], xyz[1], xyz[2]);
}

void
write_block_names(exodusIIcpp::File & exo, const YAML::Node & yml)
{
    bool have_names = false;
    std::vector<std::string> block_names;
    for (auto & n : yml) {
        if (n["name"]) {
            auto name = n["name"].as<std::string>();
            have_names = true;
            block_names.push_back(name);
        }
        else {
            std::string name = fmt::format("{}", n["id"].as<int>());
            block_names.push_back(name);
        }
    }
    if (have_names)
        exo.write_block_names(block_names);
}

void
write_element_blocks(exodusIIcpp::File & exo, const YAML::Node & yml)
{
    for (auto & blk : yml) {
        int64_t id = blk["id"].as<int>();
        auto element_type = blk["element-type"].as<std::string>();
        int64_t n_elems_in_block = 0;
        std::vector<int> connect;
        for (auto & m : blk["connectivity"]) {
            auto elem = m.as<std::vector<int>>();
            // TODO: check that number of nodes matches the `element-type`
            n_elems_in_block++;
            connect.insert(connect.end(), elem.begin(), elem.end());
        }
        exo.write_block(id, element_type.c_str(), n_elems_in_block, connect);
    }
}

void
write_side_set_names(exodusIIcpp::File & exo, const YAML::Node & yml)
{
    bool have_names = false;
    std::vector<std::string> ss_names;
    for (auto & n : yml) {
        if (n["name"]) {
            auto name = n["name"].as<std::string>();
            have_names = true;
            ss_names.push_back(name);
        }
        else {
            std::string name = fmt::format("{}", n["id"].as<int>());
            ss_names.push_back(name);
        }
    }
    if (have_names)
        exo.write_side_set_names(ss_names);
}

void
write_side_sets(exodusIIcpp::File & exo, const YAML::Node & yml)
{
    for (auto & ss : yml) {
        int64_t id = ss["id"].as<int>();
        auto name = ss["name"].as<std::string>();
        std::vector<int> elem_list;
        std::vector<int> side_list;
        for (auto & m : ss["data"]) {
            auto arr = m.as<std::vector<int>>();
            elem_list.push_back(arr[0]);
            side_list.push_back(arr[1]);
        }
        exo.write_side_set(id, elem_list, side_list);
    }
}

void
write_node_set_names(exodusIIcpp::File & exo, const YAML::Node & yml)
{
    bool have_names = false;
    std::vector<std::string> ns_names;
    for (auto & n : yml) {
        if (n["name"]) {
            auto name = n["name"].as<std::string>();
            have_names = true;
            ns_names.push_back(name);
        }
        else {
            std::string name = fmt::format("{}", n["id"].as<int>());
            ns_names.push_back(name);
        }
    }
    if (have_names)
        exo.write_node_set_names(ns_names);
}

void
write_node_sets(exodusIIcpp::File & exo, const YAML::Node & yml)
{
    for (auto & ns : yml) {
        int64_t id = ns["id"].as<int>();
        auto name = ns["name"].as<std::string>();
        std::vector<int> node_ids;
        for (auto & m : ns["data"]) {
            auto nid = m.as<int>();
            node_ids.push_back(nid);
        }
        exo.write_node_set(id, node_ids);
    }
}

void
save_exo(const std::string & exo_file_name, const YAML::Node & yml)
{
    exodusIIcpp::File exo(exo_file_name, exodusIIcpp::FileAccess::WRITE);

    int n_nodes = yml["coords"].size();
    int n_elem_blks = 0;
    int n_elems = 0;
    if (yml["element-blocks"]) {
        n_elem_blks = static_cast<int>(yml["element-blocks"].size());
        for (auto & blk : yml["element-blocks"])
            n_elems += blk["connectivity"].size();
    }
    int n_node_sets = 0;
    if (yml["node-sets"])
        n_node_sets = static_cast<int>(yml["node-sets"].size());
    int n_side_sets = 0;
    if (yml["side-sets"])
        n_side_sets = static_cast<int>(yml["side-sets"].size());
    std::string title = yml["title"] ? yml["title"].as<std::string>() : "";
    int dim = yml["dim"].as<int>();
    exo.init(title.c_str(), dim, n_nodes, n_elems, n_elem_blks, n_node_sets, n_side_sets);

    write_coordinate_names(exo, dim, yml["coord-names"]);
    write_coordinates(exo, dim, yml["coords"]);
    if (yml["element-blocks"]) {
        write_block_names(exo, yml["element-blocks"]);
        write_element_blocks(exo, yml["element-blocks"]);
    }
    if (yml["side-sets"]) {
        write_side_set_names(exo, yml["side-sets"]);
        write_side_sets(exo, yml["side-sets"]);
    }
    if (yml["node-sets"]) {
        write_node_set_names(exo, yml["node-sets"]);
        write_node_sets(exo, yml["node-sets"]);
    }
}

void
yml2exo(const std::string & yml_file_name, const std::string & exo_file_name)
{
    try {
        YAML::Node yml = load_yaml(yml_file_name);
        if (yml["exodusII"])
            save_exo(exo_file_name, yml["exodusII"]);
        else
            error("The YML file is missing the root node 'exodusII'.");
    }
    catch (YAML::Exception & e) {
        fmt::print("{}\n", e.what());
    }
}

int
main(int argc, char * argv[])
{
    cxxopts::Options opts("yml2exo");
    opts.add_option("", "h", "help", "Show this help page", cxxopts::value<bool>(), "");
    opts.add_option("", "", "yml-file", "The YAML file name", cxxopts::value<std::string>(), "");
    opts.add_option("",
                    "",
                    "exo-file",
                    "The ExodusII file name",
                    cxxopts::value<std::string>(),
                    "");

    opts.positional_help("<yml-file> <exo-file>");

    opts.parse_positional({ "yml-file", "exo-file" });
    auto res = opts.parse(argc, argv);
    if (res.count("help"))
        fmt::print("{}", opts.help());
    else if (res.count("yml-file") && res.count("exo-file"))
        yml2exo(res["yml-file"].as<std::string>(), res["exo-file"].as<std::string>());
    else
        fmt::print("{}", opts.help());

    return 0;
}
