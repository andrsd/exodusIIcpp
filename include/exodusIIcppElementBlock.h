#pragma once

#include <string>
#include <vector>

namespace exodusIIcpp {

/// Represents an element block
///
/// Element block is a connectivity array with an ID and optionally a name.
/// Each element block can have only one type of elements in it.
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

    /// Get element block ID
    ///
    /// @return Node set ID
    int get_id() const;

    /// Get element block name
    ///
    /// @return Element block name
    const std::string & get_name() const;

    /// Get the number of elements in the block
    ///
    /// @return Element block size
    int get_size() const;

    /// Get the number of nodes per element
    ///
    /// @return The number of nodes per element
    int get_num_nodes_per_element() const;

    /// Get the type of elements in the element block
    ///
    /// @return The type of the elements in this block
    const std::string & get_element_type() const;

    /// Get the element connectivity
    ///
    /// @param element_idx Index of an element in the block. `0..<n>`, where `<n>`
    /// is the size of the block. @see get_size
    /// @return List of node IDs that comprise the element
    std::vector<int> get_element_connectivity(std::size_t element_idx) const;

    /// Get the number of elements in this element block
    ///
    /// @return The number of elements in this element block
    int get_num_elements() const;

    /// Get connectivity of the block
    ///
    /// @return Connectivity array
    const std::vector<int> & get_connectivity() const;

    /// Set element block ID
    ///
    /// @param id Desired ID of the element block
    void set_id(int id);

    /// Set element block name
    ///
    /// @param name Desired name of the element block
    void set_name(const std::string & name);

    /// Set block connectivity
    ///
    /// @param elem_type Element type.
    /// @param n_elems_in_block Number of elements in the block
    /// @param n_nodes_per_elem Number of nodes per element
    /// @param connect Connectivity array. Must contain `n_elems_in_block * n_nodes_per_elem`
    /// numbers. Elements must be ordered as follows `[el1_n1, el1_n2, ..., el2_n1, el2_n2, ... ]`
    void set_connectivity(const char * elem_type,
                          int n_elems_in_block,
                          int n_nodes_per_elem,
                          const std::vector<int> & connect);
};

} // namespace exodusIIcpp
