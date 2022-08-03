#pragma once

#include <string>
#include <vector>

namespace exodusIIcpp {

class ElementBlock {
protected:
    /// Block name
    std::string name;
    /// Block ID
    int id;
    /// Number of elements
    int n_elems;
    /// Number of nodes per elements
    int n_nodes_per_elem;
    /// Element type
    std::string elem_type;
    /// Connectivity array
    std::vector<int> connect;

public:
    ElementBlock();

    int get_id() const;

    const std::string & get_name() const;

    int get_size() const;

    int get_num_nodes_per_element() const;

    const std::string & get_element_type() const;

    std::vector<int> get_element_connectivity(std::size_t element_idx) const;

    void set_id(int id);

    void set_name(const std::string & name);

    void set_connectivity(const char * elem_type,
                          int n_elems_in_block,
                          int n_nodes_per_elem,
                          const std::vector<int> & connect);
};

} // namespace exodusIIcpp
