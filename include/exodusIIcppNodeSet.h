#pragma once

#include <string>
#include <vector>

namespace exodusIIcpp {

/// Node set
///
class NodeSet {
protected:
    /// Node set name
    std::string name;
    /// Node set ID
    int id;
    /// Node IDs
    std::vector<int> node_ids;

public:
    NodeSet();

    int get_id() const;

    const std::string & get_name() const;

    int get_size() const;

    int get_node_id(std::size_t idx) const;

    void set_id(int id);

    void set_name(const std::string & name);

    void set_nodes(const std::vector<int> & nodes);
};

} // namespace exodusIIcpp
