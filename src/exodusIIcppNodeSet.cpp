#include "exodusIIcppNodeSet.h"
#include <stdexcept>

namespace exodusIIcpp {

NodeSet::NodeSet() : id(-1) {}

int
NodeSet::get_id() const
{
    return this->id;
}

const std::string &
NodeSet::get_name() const
{
    return this->name;
}

int
NodeSet::get_size() const
{
    return this->node_ids.size();
}

int
NodeSet::get_node_id(std::size_t idx) const
{
    if (idx < this->node_ids.size())
        return this->node_ids[idx];
    else
        throw std::out_of_range("Index of out bounds.");
}

void
NodeSet::set_id(int id)
{
    this->id = id;
}

void
NodeSet::set_name(const std::string & name)
{
    this->name = name;
}

void
NodeSet::set_nodes(const std::vector<int> & nodes)
{
    this->node_ids = nodes;
}

} // namespace exodusIIcpp
