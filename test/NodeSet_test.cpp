#include "gmock/gmock.h"
#include "exodusIIcpp.h"
#include <stdexcept>

using namespace exodusIIcpp;
using namespace testing;

TEST(NodeSetTest, test)
{
    NodeSet ns;
    EXPECT_EQ(ns.get_id(), -1);
    EXPECT_EQ(ns.get_size(), 0);

    ns.set_id(201);
    EXPECT_EQ(ns.get_id(), 201);

    ns.set_name("node_set");
    EXPECT_STREQ(ns.get_name().c_str(), "node_set");

    std::vector<int> nodes = { 1, 2, 3 };
    ns.set_nodes(nodes);
    EXPECT_EQ(ns.get_size(), 3);

    EXPECT_THAT(ns.get_node_id(0), 1);
    EXPECT_THAT(ns.get_node_id(1), 2);
    EXPECT_THAT(ns.get_node_id(2), 3);

    EXPECT_THROW(ns.get_node_id(3), Exception);

    EXPECT_THAT(ns.get_node_ids(), ElementsAre(1, 2, 3));
}
