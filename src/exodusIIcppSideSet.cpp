// SPDX-FileCopyrightText: 2022 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#include "exodusIIcppSideSet.h"
#include "exodusIIcppException.h"

namespace exodusIIcpp {

SideSet::SideSet() : id(-1) {}

int
SideSet::get_id() const
{
    return this->id;
}

const std::string &
SideSet::get_name() const
{
    return this->name;
}

int
SideSet::get_size() const
{
    return this->elem_ids.size();
}

int
SideSet::get_element_id(std::size_t idx) const
{
    if (idx < this->elem_ids.size())
        return this->elem_ids[idx];
    else
        throw Exception("Index of out bounds.");
}

int
SideSet::get_side_id(std::size_t idx) const
{
    if (idx < this->side_ids.size())
        return this->side_ids[idx];
    else
        throw Exception("Index of out bounds.");
}

void
SideSet::set_id(int id)
{
    this->id = id;
}

void
SideSet::set_name(const std::string & name)
{
    this->name = name;
}

void
SideSet::set_sides(const std::vector<int> & elems, const std::vector<int> & sides)
{
    if (elems.size() == sides.size()) {
        this->elem_ids = elems;
        this->side_ids = sides;
    }
    else
        throw Exception("The length of 'elems' must be equal to the length of 'sides'");
}

const std::vector<int> &
SideSet::get_element_ids() const
{
    return this->elem_ids;
}

const std::vector<int> &
SideSet::get_side_ids() const
{
    return this->side_ids;
}

void
SideSet::add(int elem, int side)
{
    this->elem_ids.push_back(elem);
    this->side_ids.push_back(side);
}

} // namespace exodusIIcpp
