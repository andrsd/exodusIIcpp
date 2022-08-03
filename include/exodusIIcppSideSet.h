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

    int get_id() const;

    const std::string & get_name() const;

    int get_size() const;

    int get_element_id(std::size_t idx) const;

    int get_side_id(std::size_t idx) const;

    void set_id(int id);

    void set_name(const std::string & name);

    void set_sides(const std::vector<int> & elems, const std::vector<int> & sides);
};

} // namespace exodusIIcpp
