#include "gmock/gmock.h"
#include "exodusIIcpp.h"
#include <stdexcept>

using namespace exodusIIcpp;

TEST(ElementBlockTest, test)
{
    ElementBlock eb;
    EXPECT_EQ(eb.get_id(), -1);
    EXPECT_EQ(eb.get_size(), -1);
    EXPECT_EQ(eb.get_num_nodes_per_element(), -1);

    eb.set_id(101);
    EXPECT_EQ(eb.get_id(), 101);

    eb.set_name("block");
    EXPECT_STREQ(eb.get_name().c_str(), "block");

    std::vector<int> connect = { 1, 2, 2, 3, 3, 4 };
    eb.set_connectivity("BAR2", 3, 2, connect);
    EXPECT_STREQ(eb.get_element_type().c_str(), "BAR2");
    EXPECT_EQ(eb.get_num_elements(), 3);
    EXPECT_EQ(eb.get_num_nodes_per_element(), 2);
    EXPECT_EQ(eb.get_size(), 3);

    EXPECT_THAT(eb.get_element_connectivity(0), testing::ElementsAre(1, 2));
    EXPECT_THAT(eb.get_element_connectivity(1), testing::ElementsAre(2, 3));
    EXPECT_THAT(eb.get_element_connectivity(2), testing::ElementsAre(3, 4));

    EXPECT_THAT(eb.get_connectivity(), testing::ElementsAre(1, 2, 2, 3, 3, 4));

    EXPECT_THROW(eb.get_element_connectivity(3), std::out_of_range);
}
