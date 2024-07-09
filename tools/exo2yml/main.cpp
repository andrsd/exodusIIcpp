#include "cxxopts/cxxopts.hpp"
#include "fmt/printf.h"
#include "yaml-cpp/yaml.h"
#include "exodusIIcpp/exodusIIcpp.h"
#include <fstream>

exodusIIcpp::File
load_exo(const std::string & file_name)
{
    exodusIIcpp::File file(file_name, exodusIIcpp::FileAccess::READ);
    return file;
}

void
write_header(YAML::Emitter & yml, exodusIIcpp::File & exo)
{
    yml << YAML::Key << "title";
    yml << YAML::Value << exo.get_title();

    yml << YAML::Key << "dim";
    yml << YAML::Value << exo.get_dim();
}

void
write_coordinates(YAML::Emitter & yml, exodusIIcpp::File & exo)
{
    auto dim = exo.get_dim();
    exo.read_coords();
    if (dim == 1) {
        const std::vector<double> & x = exo.get_x_coords();
        yml << YAML::Key << "coords";
        yml << YAML::Value;
        yml << YAML::BeginSeq;
        for (std::size_t i = 0; i < x.size(); i++) {
            yml << YAML::Flow;
            yml << YAML::BeginSeq;
            yml << x[i];
            yml << YAML::EndSeq;
        }
        yml << YAML::EndSeq;
    }
    else if (dim == 2) {
        const std::vector<double> & x = exo.get_x_coords();
        const std::vector<double> & y = exo.get_y_coords();
        yml << YAML::Key << "coords";
        yml << YAML::Value;
        yml << YAML::BeginSeq;
        for (std::size_t i = 0; i < x.size(); i++) {
            yml << YAML::Flow;
            yml << YAML::BeginSeq;
            yml << x[i] << y[i];
            yml << YAML::EndSeq;
        }
        yml << YAML::EndSeq;
    }
    else if (dim == 3) {
        const std::vector<double> & x = exo.get_x_coords();
        const std::vector<double> & y = exo.get_y_coords();
        const std::vector<double> & z = exo.get_z_coords();
        yml << YAML::Key << "coords";
        yml << YAML::Value;
        yml << YAML::BeginSeq;
        for (std::size_t i = 0; i < x.size(); i++) {
            yml << YAML::Flow;
            yml << YAML::BeginSeq;
            yml << x[i] << y[i] << z[i];
            yml << YAML::EndSeq;
        }
        yml << YAML::EndSeq;
    }
}

void
write_coordinate_names(YAML::Emitter & yml, exodusIIcpp::File & exo)
{
    exo.read_coord_names();
    auto coord_names = exo.get_coord_names();
    yml << YAML::Key << "coord-names";
    yml << YAML::Value;
    yml << YAML::BeginSeq;
    yml << YAML::Flow;
    yml << coord_names;
    yml << YAML::EndSeq;
}

void
write_element_block(YAML::Emitter & yml, exodusIIcpp::File & exo)
{
    exo.read_blocks();
    auto el_blocks = exo.get_element_blocks();
    yml << YAML::Key << "element-blocks";
    yml << YAML::Value;
    yml << YAML::BeginSeq;
    for (auto & blk : el_blocks) {
        yml << YAML::BeginMap;

        yml << YAML::Key << "id";
        yml << YAML::Value << blk.get_id();

        if (!blk.get_name().empty()) {
            yml << YAML::Key << "name";
            yml << YAML::Value << blk.get_name();
        }

        yml << YAML::Key << "element-type";
        yml << YAML::Value << blk.get_element_type();

        yml << YAML::Key << "connectivity";
        yml << YAML::Value;
        yml << YAML::BeginSeq;
        for (size_t e = 0; e < blk.get_num_elements(); e++) {
            std::vector<int> connect = blk.get_element_connectivity(e);
            yml << YAML::Flow;
            yml << connect;
        }
        yml << YAML::EndSeq;

        yml << YAML::EndMap;
    }
    yml << YAML::EndSeq;
}

void
write_side_sets(YAML::Emitter & yml, exodusIIcpp::File & exo)
{
    exo.read_side_sets();
    auto side_sets = exo.get_side_sets();
    yml << YAML::Key << "side-sets";
    yml << YAML::Value;
    yml << YAML::BeginSeq;
    for (auto & ss : side_sets) {
        yml << YAML::BeginMap;

        yml << YAML::Key << "id";
        yml << YAML::Value << ss.get_id();

        if (!ss.get_name().empty()) {
            yml << YAML::Key << "name";
            yml << YAML::Value << ss.get_name();
        }

        yml << YAML::Key << "data";
        yml << YAML::Value;
        yml << YAML::BeginSeq;
        for (size_t i = 0; i < ss.get_size(); i++) {
            yml << YAML::Flow;
            std::vector<int> pair = { ss.get_element_id(i), ss.get_side_id(i) };
            yml << pair;
        }
        yml << YAML::EndSeq;

        yml << YAML::EndMap;
    }
    yml << YAML::EndSeq;
}

void
write_node_sets(YAML::Emitter & yml, exodusIIcpp::File & exo)
{
    exo.read_node_sets();
    auto node_sets = exo.get_node_sets();
    yml << YAML::Key << "node-sets";
    yml << YAML::Value;
    yml << YAML::BeginSeq;
    for (auto & ns : node_sets) {
        yml << YAML::BeginMap;

        yml << YAML::Key << "id";
        yml << YAML::Value << ns.get_id();

        if (!ns.get_name().empty()) {
            yml << YAML::Key << "name";
            yml << YAML::Value << ns.get_name();
        }

        yml << YAML::Key << "data";
        yml << YAML::Value;
        yml << YAML::Flow;
        yml << YAML::BeginSeq;
        for (size_t i = 0; i < ns.get_size(); i++)
            yml << ns.get_node_id(i);
        yml << YAML::EndSeq;

        yml << YAML::EndMap;
    }
    yml << YAML::EndSeq;
}

void
save_yml(const std::string & file_name, exodusIIcpp::File & exo)
{
    std::ofstream out;
    out.open(file_name);
    YAML::Emitter yml(out);

    yml << YAML::BeginMap;
    yml << YAML::Key << "exodusII";
    yml << YAML::Value;

    exo.init();
    yml << YAML::BeginMap;
    write_header(yml, exo);
    write_coordinate_names(yml, exo);
    write_coordinates(yml, exo);
    write_element_block(yml, exo);
    write_side_sets(yml, exo);
    write_node_sets(yml, exo);
    yml << YAML::EndMap;

    yml << YAML::EndMap;

    out.close();
}

void
yml2exo(const std::string & yml_file_name, const std::string & exo_file_name)
{
    try {
        auto exo = load_exo(exo_file_name);
        if (exo.is_opened())
            save_yml(yml_file_name, exo);
    }
    catch (std::runtime_error & e) {
        fmt::print("{}\n", e.what());
    }
}

int
main(int argc, char * argv[])
{
    cxxopts::Options opts("exo2yml");
    opts.add_option("", "h", "help", "Show this help page", cxxopts::value<bool>(), "");
    opts.add_option("", "", "yml-file", "The YAML file name", cxxopts::value<std::string>(), "");
    opts.add_option("",
                    "",
                    "exo-file",
                    "The ExodusII file name",
                    cxxopts::value<std::string>(),
                    "");

    opts.positional_help("<exo-file> <yml-file>");

    opts.parse_positional({ "exo-file", "yml-file" });
    auto res = opts.parse(argc, argv);
    if (res.count("yml-file") && res.count("exo-file"))
        yml2exo(res["yml-file"].as<std::string>(), res["exo-file"].as<std::string>());
    else
        fmt::print("{}", opts.help());

    return 0;
}
