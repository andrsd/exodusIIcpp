#include "gmock/gmock.h"
#include "exodusIIcpp.h"
#include <stdexcept>

using namespace exodusIIcpp;
using namespace testing;

TEST(SideSetTest, test)
{
    SideSet ss;
    EXPECT_EQ(ss.get_id(), -1);
    EXPECT_EQ(ss.get_size(), 0);

    ss.set_id(301);
    EXPECT_EQ(ss.get_id(), 301);

    ss.set_name("side_set");
    EXPECT_STREQ(ss.get_name().c_str(), "side_set");

    std::vector<int> elems = { 1, 2, 3 };
    std::vector<int> sides = { 0, 1, 0 };
    ss.set_sides(elems, sides);
    EXPECT_EQ(ss.get_size(), 3);

    EXPECT_THAT(ss.get_element_id(0), 1);
    EXPECT_THAT(ss.get_element_id(1), 2);
    EXPECT_THAT(ss.get_element_id(2), 3);
    EXPECT_THROW(ss.get_element_id(3), std::out_of_range);
    EXPECT_THAT(ss.get_element_ids(), ElementsAre(1, 2, 3));

    EXPECT_THAT(ss.get_side_id(0), 0);
    EXPECT_THAT(ss.get_side_id(1), 1);
    EXPECT_THAT(ss.get_side_id(2), 0);
    EXPECT_THROW(ss.get_side_id(3), std::out_of_range);
    EXPECT_THAT(ss.get_side_ids(), ElementsAre(0, 1, 0));

    ss.add(4, 1);
    EXPECT_EQ(ss.get_element_id(3), 4);
    EXPECT_EQ(ss.get_side_id(3), 1);
}

TEST(SideSetTest, set_sides_oob)
{
    SideSet ss;
    std::vector<int> elems = { 1, 2 };
    std::vector<int> sides = { 0, 1, 0 };
    EXPECT_THROW(ss.set_sides(elems, sides), std::logic_error);
}
