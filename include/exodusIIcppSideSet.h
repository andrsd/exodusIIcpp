// SPDX-FileCopyrightText: 2022 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <string>
#include <vector>

namespace exodusIIcpp {

/// Side set
///
class SideSet {
protected:
    /// Face set name
    std::string name;
    /// Face set ID
    int id;
    /// Element IDs
    std::vector<int> elem_ids;
    /// Local side IDs
    std::vector<int> side_ids;

public:
    SideSet();

    /// Get side set ID
    ///
    /// @return Side set ID
    int get_id() const;

    /// Get side set name
    ///
    /// @return Side set name
    const std::string & get_name() const;

    /// Get the number of sides in the set
    ///
    /// @return Side set size
    int get_size() const;

    /// Get an element ID of a side in the side set
    ///
    /// @param idx Index of the side. Can be `0..<size of the side set>`.
    /// @return Element ID
    /// @see get_size
    int get_element_id(std::size_t idx) const;

    /// Get element IDs
    ///
    /// @return Array of element IDs
    const std::vector<int> & get_element_ids() const;

    /// Get side IDs
    ///
    /// @return Array of side IDs
    const std::vector<int> & get_side_ids() const;

    /// Get an local side number of a side in the side set
    ///
    /// @param idx Index of the side. Can be `0..<size of the side set>`.
    /// @return Local side number
    /// @see get_size
    int get_side_id(std::size_t idx) const;

    /// Set side set ID
    ///
    /// @param id Desired ID of the side set
    void set_id(int id);

    /// Set side set name
    ///
    /// @param name Desired name of the side set
    void set_name(const std::string & name);

    /// Set side set sides
    ///
    /// @param elems List of element IDs that will comprise the side set
    /// @param sides List of local side numbers that will comprise the side set
    ///
    /// @note Size of `elems` and `sides` should be equal.
    void set_sides(const std::vector<int> & elems, const std::vector<int> & sides);

    /// Add an (elem_id, side_id) pair into the side set
    ///
    /// @param elem Element ID
    /// @param side Local side number
    void add(int elem, int side);
};

} // namespace exodusIIcpp
