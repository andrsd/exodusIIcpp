#include "gmock/gmock.h"
#include "exodusIIcpp/exodusIIcpp.h"

using namespace exodusIIcpp;
using namespace testing;

TEST(FileTest, empty)
{
    File f;

    EXPECT_FALSE(f.is_opened());
    EXPECT_EQ(f.get_dim(), -1);
    EXPECT_EQ(f.get_num_nodes(), -1);
    EXPECT_EQ(f.get_num_elements(), -1);
    EXPECT_EQ(f.get_num_element_blocks(), -1);
    EXPECT_EQ(f.get_num_node_sets(), -1);
    EXPECT_EQ(f.get_num_side_sets(), -1);
}

TEST(FileTest, open)
{
    File f(std::string(EXODUSIICPP_UNIT_TEST_ASSETS) + std::string("/tri.e"), FileAccess::READ);
    EXPECT_THROW({ f.init("t", 2, 3, 1, 1, 0, 0); }, Exception);

    EXPECT_TRUE(f.is_opened());
    EXPECT_EQ(f.get_dim(), 2);
    EXPECT_EQ(f.get_num_nodes(), 3);
    EXPECT_EQ(f.get_num_elements(), 1);
    EXPECT_EQ(f.get_num_element_blocks(), 1);
    EXPECT_EQ(f.get_title(), "Created by meshio v5.3.4, 2022-04-25T17:29:15.676538");

    f.read_elem_map();
    // TODO: add a check that map was read and is ok
}

TEST(FileTest, open_non_existing)
{
    EXPECT_THROW(
        {
            File f(std::string(EXODUSIICPP_UNIT_TEST_ASSETS) + std::string("/non-existent-file.e"),
                   FileAccess::READ);
        },
        Exception);
}

TEST(FileTest, create_tet4)
{
    File f(std::string("tet4.e"), FileAccess::WRITE);
    EXPECT_TRUE(f.is_opened());
    f.init("test", 3, 4, 1, 1, 2, 1);

    std::vector<std::string> nfo = { "test1", "test2" };
    f.write_info(nfo);

    std::vector<double> x = { 0, 1, 0, 0 };
    std::vector<double> y = { 0, 0, 1, 0 };
    std::vector<double> z = { 0, 0, 1, 1 };
    f.write_coords(x, y, z);
    f.write_coord_names();

    std::vector<int> connect1 = { 1, 2, 3, 4 };
    f.write_block(1, "TET4", 1, connect1);
    std::vector<std::string> blk_names = { "blk1" };
    f.write_block_names(blk_names);

    std::vector<int> elem_list = { 1 };
    std::vector<int> side_list = { 0 };
    f.write_side_set(1, elem_list, side_list);
    std::vector<std::string> ss_names = { "sideset1" };
    f.write_side_set_names(ss_names);

    std::vector<int> elem_list_err = { 1 };
    std::vector<int> side_list_err = {};
    EXPECT_THROW({ f.write_side_set(2, elem_list_err, side_list_err); }, Exception);

    std::vector<int> ns1 = { 2 };
    f.write_node_set(1, ns1);
    std::vector<int> ns2 = { 3 };
    f.write_node_set(2, ns2);
    std::vector<std::string> ns_names = { "nodeset1", "nodeset2" };
    f.write_node_set_names(ns_names);

    f.write_global_var_names({});

    f.close();

    // check the file that we created
    File g(std::string("tet4.e"), FileAccess::READ);
    g.read();
    EXPECT_EQ(g.get_dim(), 3);
    EXPECT_EQ(g.get_num_nodes(), 4);
    EXPECT_EQ(g.get_num_elements(), 1);
    EXPECT_EQ(g.get_num_element_blocks(), 1);
    EXPECT_EQ(g.get_num_node_sets(), 2);
    EXPECT_EQ(g.get_num_side_sets(), 1);

    const std::vector<double> & gx = g.get_x_coords();
    EXPECT_THAT(gx, testing::ElementsAre(0., 1., 0., 0.));
    const std::vector<double> & gy = g.get_y_coords();
    EXPECT_THAT(gy, testing::ElementsAre(0., 0., 1., 0.));
    const std::vector<double> & gz = g.get_z_coords();
    EXPECT_THAT(gz, testing::ElementsAre(0., 0., 1., 1.));

    const std::vector<ElementBlock> & blocks = g.get_element_blocks();
    EXPECT_EQ(blocks.size(), 1);

    const ElementBlock & eb1 = g.get_element_block(0);
    EXPECT_EQ(eb1.get_size(), 1);
    EXPECT_EQ(eb1.get_id(), 1);

    EXPECT_THROW({ g.get_element_block(9999); }, Exception);

    const std::vector<SideSet> & side_sets = g.get_side_sets();
    EXPECT_EQ(side_sets.size(), 1);

    const std::vector<NodeSet> & node_sets = g.get_node_sets();
    EXPECT_EQ(node_sets.size(), 2);
}

TEST(FileTest, create_tri3)
{
    File f(std::string("tri3.e"), FileAccess::WRITE);
    EXPECT_TRUE(f.is_opened());
    EXPECT_THROW({ f.init(); }, Exception);
    f.init("test", 2, 3, 1, 1, 0, 0);

    std::vector<double> x = { 0, 1, 0 };
    std::vector<double> y = { 0, 0, 1 };
    f.write_coords(x, y);
    f.write_coord_names();

    std::vector<int> connect1 = { 1, 2, 3 };
    f.write_block(1, "TRI3", 1, connect1);
    std::vector<std::string> blk_names = { "blk1" };
    f.write_block_names(blk_names);

    f.write_side_set_names({});

    f.write_time(1, 1.);

    std::vector<std::string> nv_names = { "nv1" };
    f.write_nodal_var_names(nv_names);
    for (int i = 0; i < 3; i++)
        f.write_partial_nodal_var(1, 1, 1, i + 1, 2 * i);

    std::vector<std::string> ev_names = { "ev1" };
    f.write_elem_var_names(ev_names);
    f.write_partial_elem_var(1, 1, 1, 1, 12);

    std::vector<std::string> gv_names = { "gv1" };
    f.write_global_var_names(gv_names);
    f.write_global_var(1, 1, 89);

    f.update();

    f.close();
}

TEST(FileTest, create_edge2)
{
    File f(std::string("edg2.e"), FileAccess::WRITE);
    EXPECT_TRUE(f.is_opened());
    f.init("test", 1, 3, 2, 1, 0, 0);

    std::vector<double> x = { 0, 1, 2 };
    f.write_coords(x);

    std::vector<int> connect1 = { 1, 2, 2, 3 };
    f.write_block(1, "BAR2", 2, connect1);

    f.write_time(1, 1.);

    std::vector<std::string> nv_names = { "nv1" };
    f.write_nodal_var_names(nv_names);
    f.write_nodal_var(1, 1, { 10, 11, 12 });
    f.update();

    f.close();

    // check the file that we created
    File g(std::string("edg2.e"), FileAccess::READ);
    g.read();
    EXPECT_EQ(g.get_dim(), 1);
    EXPECT_EQ(g.get_num_nodes(), 3);
    EXPECT_EQ(g.get_num_elements(), 2);
    EXPECT_EQ(g.get_num_element_blocks(), 1);
    EXPECT_EQ(g.get_num_node_sets(), 0);
    EXPECT_EQ(g.get_num_side_sets(), 0);
}

TEST(FileTest, read_square)
{
    File f(std::string(EXODUSIICPP_UNIT_TEST_ASSETS) + std::string("/square.e"), FileAccess::READ);
    if (f.is_opened()) {
        std::map<int, std::string> blk_names = f.read_block_names();
        EXPECT_EQ(blk_names.size(), 1);
        EXPECT_THAT(blk_names, ElementsAre(Pair(0, "")));

        std::map<int, std::string> ss_names = f.read_side_set_names();
        EXPECT_EQ(ss_names.size(), 4);
        EXPECT_THAT(
            ss_names,
            ElementsAre(Pair(0, "bottom"), Pair(1, "right"), Pair(2, "top"), Pair(3, "left")));

        std::map<int, std::string> ns_names = f.read_node_set_names();
        EXPECT_EQ(ns_names.size(), 4);
        EXPECT_THAT(
            ns_names,
            ElementsAre(Pair(0, "bottom"), Pair(1, "right"), Pair(2, "top"), Pair(3, "left")));

        f.read_times();
        auto times = f.get_times();
        EXPECT_EQ(times.size(), 2);
        EXPECT_THAT(times, ElementsAre(0., 1.));

        auto nodal_var_names = f.get_nodal_variable_names();
        EXPECT_EQ(nodal_var_names.size(), 1);
        EXPECT_THAT(nodal_var_names, ElementsAre("u"));

        auto elem_var_names = f.get_elemental_variable_names();
        EXPECT_EQ(elem_var_names.size(), 0);

        auto global_var_names = f.get_global_variable_names();
        EXPECT_EQ(global_var_names.size(), 0);

        int var_idx = 1;
        auto u_var_0 = f.get_nodal_variable_values(1, var_idx);
        EXPECT_THAT(u_var_0,
                    ElementsAre(DoubleEq(0.),
                                DoubleEq(0.),
                                DoubleEq(0.),
                                DoubleEq(0.),
                                DoubleEq(0.),
                                DoubleEq(0.),
                                DoubleEq(0.),
                                DoubleEq(0.),
                                DoubleEq(0.)));
        auto u_var_1 = f.get_nodal_variable_values(2, var_idx);
        EXPECT_THAT(u_var_1,
                    ElementsAre(DoubleEq(0.),
                                DoubleEq(0.5),
                                DoubleEq(0.5),
                                DoubleEq(0.),
                                DoubleEq(1.),
                                DoubleEq(1.),
                                DoubleEq(0.5),
                                DoubleEq(0.),
                                DoubleEq(1.)));

        f.close();
    }
}

TEST(FileTest, custom_coord_names)
{
    File f(std::string("coords.e"), FileAccess::WRITE);
    EXPECT_TRUE(f.is_opened());
    f.init("test", 2, 3, 0, 0, 0, 0);

    std::vector<double> x = { 0, 1, 0 };
    std::vector<double> y = { 0, 0, 1 };
    f.write_coords(x, y);
    f.write_coord_names({ "r", "z" });
    f.close();

    // check the file that we created
    File g(std::string("coords.e"), FileAccess::READ);
    g.read();
    EXPECT_THAT(g.get_coord_names(), ElementsAre("r", "z"));
}

TEST(FileTest, test)
{
    File f(std::string(EXODUSIICPP_UNIT_TEST_ASSETS) + std::string("/test.exo"), FileAccess::READ);
    if (f.is_opened()) {
        auto global_var_names = f.get_global_variable_names();
        EXPECT_EQ(global_var_names.size(), 1);
        EXPECT_THAT(global_var_names, ElementsAre("glo_vars"));

        auto vals = f.get_global_variable_values(1);
        EXPECT_THAT(vals, ElementsAre(DoubleEq(0.02)));
        vals = f.get_global_variable_values(5);
        EXPECT_THAT(vals, ElementsAre(DoubleEq(0.1)));
        vals = f.get_global_variable_values(10);
        EXPECT_THAT(vals, ElementsAre(DoubleEq(0.2)));

        vals = f.get_global_variable_values(1, 1);
        EXPECT_THAT(vals,
                    ElementsAre(DoubleEq(0.02),
                                DoubleEq(0.04),
                                DoubleEq(0.06),
                                DoubleEq(0.08),
                                DoubleEq(0.1),
                                DoubleEq(0.12),
                                DoubleEq(0.14),
                                DoubleEq(0.16),
                                DoubleEq(0.18),
                                DoubleEq(0.2)));

        vals = f.get_global_variable_values(1, 4, 7);
        EXPECT_THAT(vals,
                    ElementsAre(DoubleEq(0.08), DoubleEq(0.10), DoubleEq(0.12), DoubleEq(0.14)));

        auto elem_var_names = f.get_elemental_variable_names();
        EXPECT_THAT(elem_var_names, ElementsAre("ele_var0", "ele_var1", "ele_var2"));

        auto ev3b2_vals = f.get_elemental_variable_values(10, 3, 11);
        EXPECT_THAT(ev3b2_vals, ElementsAre(DoubleEq(7.1)));

        std::vector<int> ss_cnts;
        std::vector<int> ss_nodes;
        f.get_side_set_node_list(31, ss_cnts, ss_nodes);
        EXPECT_THAT(ss_cnts, ElementsAre(2, 2));
        EXPECT_THAT(ss_nodes, ElementsAre(2, 3, 7, 8));

        f.close();
    }
}
