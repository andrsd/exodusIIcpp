// SPDX-FileCopyrightText: 2022 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <string>
#include <vector>

namespace exodusIIcpp {

/// Node set
///
/// Represents a node set stored in/loaded from the ExodusII file
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

    /// Get node set ID
    ///
    /// @return Node set ID
    int get_id() const;

    /// Get node set name
    ///
    /// @return Node set name
    const std::string & get_name() const;

    /// Get the number of nodes in the set
    ///
    /// @return Node set size
    int get_size() const;

    /// Get an ID of a node in the node set
    ///
    /// @param idx Index of the node. Can be `0..<size of the node set>`.
    /// @return Node ID
    /// @see get_size
    int get_node_id(std::size_t idx) const;

    /// Get node IDs contained in this node set
    ///
    /// @return Node IDs contained in this node set
    const std::vector<int> & get_node_ids() const;

    /// Set node set ID
    ///
    /// @param id Desired ID of the node set
    void set_id(int id);

    /// Set node set name
    ///
    /// @param name Desired name of the node set
    void set_name(const std::string & name);

    /// Set node set nodes
    ///
    /// @param nodes List of node IDs that will comprise the node set
    void set_nodes(const std::vector<int> & nodes);
};

} // namespace exodusIIcpp
