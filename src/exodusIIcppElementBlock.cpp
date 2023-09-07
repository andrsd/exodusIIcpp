#include "exodusIIcppElementBlock.h"
#include "exodusIIcppException.h"

namespace exodusIIcpp {

ElementBlock::ElementBlock() : id(-1), n_elems(-1), n_nodes_per_elem(-1) {}

int
ElementBlock::get_id() const
{
    return this->id;
}

const std::string &
ElementBlock::get_name() const
{
    return this->name;
}

int
ElementBlock::get_size() const
{
    return this->n_elems;
}

int
ElementBlock::get_num_nodes_per_element() const
{
    return this->n_nodes_per_elem;
}

const std::string &
ElementBlock::get_element_type() const
{
    return this->elem_type;
}

const std::vector<int> &
ElementBlock::get_connectivity() const
{
    return this->connect;
}

std::vector<int>
ElementBlock::get_element_connectivity(std::size_t element_idx) const
{
    std::vector<int> elem_connect(this->n_nodes_per_elem);
    if (element_idx < this->n_elems) {
        std::size_t ofst = element_idx * this->n_nodes_per_elem;
        for (std::size_t i = 0; i < this->n_nodes_per_elem; i++)
            elem_connect[i] = this->connect[ofst + i];
        return elem_connect;
    }
    else
        throw Exception("Index of of range");
}

int
ElementBlock::get_num_elements() const
{
    return this->n_elems;
}

void
ElementBlock::set_id(int id)
{
    this->id = id;
}

void
ElementBlock::set_name(const std::string & name)
{
    this->name = name;
}

void
ElementBlock::set_connectivity(const char * elem_type,
                               int n_elems_in_block,
                               int n_nodes_per_elem,
                               const std::vector<int> & connect)
{
    this->elem_type = elem_type;
    this->n_elems = n_elems_in_block;
    this->n_nodes_per_elem = n_nodes_per_elem;
    this->connect = connect;
}

} // namespace exodusIIcpp
