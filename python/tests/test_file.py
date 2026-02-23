# SPDX-FileCopyrightText: 2026 David Andrs <andrsd@gmail.com>
# SPDX-License-Identifier: MIT

import pathlib

import exodusIIcpp
import pytest


def test_empty():
    """Test that a newly constructed File is empty."""
    f = exodusIIcpp.File()

    assert not f.is_opened()
    assert f.get_dim() == -1
    assert f.get_num_nodes() == -1
    assert f.get_num_elements() == -1
    assert f.get_num_element_blocks() == -1
    assert f.get_num_node_sets() == -1
    assert f.get_num_side_sets() == -1


def test_open(assets_dir):
    """Test opening an existing file."""
    f = exodusIIcpp.File(str(assets_dir / "tri.e"), exodusIIcpp.FileAccess.READ)

    # Calling init on a read file should raise an exception
    with pytest.raises(RuntimeError):
        f.init("t", 2, 3, 1, 1, 0, 0)

    assert f.is_opened()
    assert f.get_dim() == 2
    assert f.get_num_nodes() == 3
    assert f.get_num_elements() == 1
    assert f.get_num_element_blocks() == 1
    assert f.get_title() == "Created by meshio v5.3.4, 2022-04-25T17:29:15.676538"

    f.read_elem_map()
    f.close()


def test_open_non_existing(assets_dir):
    """Test that opening a non-existent file raises an exception."""
    f = exodusIIcpp.File()
    with pytest.raises(RuntimeError):
        f.open(str(assets_dir / "non-existent-file.e"))


def test_create_tet4(tmp_dir):
    """Test creating and reading back a TET4 mesh."""
    file_path = str(tmp_dir / "tet4.e")

    # Create file
    f = exodusIIcpp.File()
    f.create(file_path)
    assert f.is_opened()
    f.init("test", 3, 4, 1, 1, 2, 1)

    nfo = ["test1", "test2"]
    f.write_info(nfo)

    x = [0, 1, 0, 0]
    y = [0, 0, 1, 0]
    z = [0, 0, 1, 1]
    f.write_coords(x, y, z)
    f.write_coord_names()

    connect1 = [1, 2, 3, 4]
    f.write_block(1, "TET4", 1, connect1)
    blk_names = ["blk1"]
    f.write_block_names(blk_names)

    elem_list = [1]
    side_list = [0]
    f.write_side_set(1, elem_list, side_list)
    ss_names = ["sideset1"]
    f.write_side_set_names(ss_names)

    # Writing side set with mismatched list sizes should raise
    elem_list_err = [1]
    side_list_err = []
    with pytest.raises(RuntimeError):
        f.write_side_set(2, elem_list_err, side_list_err)

    ns1 = [2]
    f.write_node_set(1, ns1)
    ns2 = [3]
    f.write_node_set(2, ns2)
    ns_names = ["nodeset1", "nodeset2"]
    f.write_node_set_names(ns_names)

    f.write_global_var_names([])
    f.close()

    # Verify the created file
    g = exodusIIcpp.File(file_path, exodusIIcpp.FileAccess.READ)
    g.read()

    assert g.get_dim() == 3
    assert g.get_num_nodes() == 4
    assert g.get_num_elements() == 1
    assert g.get_num_element_blocks() == 1
    assert g.get_num_node_sets() == 2
    assert g.get_num_side_sets() == 1

    gx = g.get_x_coords()
    assert gx == [0.0, 1.0, 0.0, 0.0]
    gy = g.get_y_coords()
    assert gy == [0.0, 0.0, 1.0, 0.0]
    gz = g.get_z_coords()
    assert gz == [0.0, 0.0, 1.0, 1.0]

    blocks = g.get_element_blocks()
    assert len(blocks) == 1

    eb1 = blocks[0]
    assert eb1.get_size() == 1
    assert eb1.get_id() == 1

    with pytest.raises(RuntimeError):
        g.get_element_block(9999)

    side_sets = g.get_side_sets()
    assert len(side_sets) == 1

    node_sets = g.get_node_sets()
    assert len(node_sets) == 2

    g.close()


def test_create_tri3(tmp_dir):
    """Test creating a TRI3 mesh with variables."""
    file_path = str(tmp_dir / "tri3.e")

    f = exodusIIcpp.File()
    f.create(file_path)
    assert f.is_opened()

    with pytest.raises(RuntimeError):
        f.init()

    f.init("test", 2, 3, 1, 1, 0, 0)

    x = [0, 1, 0]
    y = [0, 0, 1]
    f.write_coords(x, y)
    f.write_coord_names()

    connect1 = [1, 2, 3]
    f.write_block(1, "TRI3", 1, connect1)
    blk_names = ["blk1"]
    f.write_block_names(blk_names)

    f.write_side_set_names([])

    f.write_time(1, 1.0)

    nv_names = ["nv1"]
    f.write_nodal_var_names(nv_names)
    for i in range(3):
        f.write_partial_nodal_var(1, 1, 1, i + 1, 2 * i)

    ev_names = ["ev1"]
    f.write_elem_var_names(ev_names)
    f.write_partial_elem_var(1, 1, 1, 1, 12)

    gv_names = ["gv1"]
    f.write_global_var_names(gv_names)
    f.write_global_var(1, 1, 89)

    f.update()
    f.close()


def test_create_edge2(tmp_dir):
    """Test creating a 1D mesh (BAR2 elements)."""
    file_path = str(tmp_dir / "edg2.e")

    f = exodusIIcpp.File()
    f.create(file_path)
    assert f.is_opened()
    f.init("test", 1, 3, 2, 1, 0, 0)

    x = [0, 1, 2]
    f.write_coords(x)

    connect1 = [1, 2, 2, 3]
    f.write_block(1, "BAR2", 2, connect1)

    f.write_time(1, 1.0)

    nv_names = ["nv1"]
    f.write_nodal_var_names(nv_names)
    f.write_nodal_var(1, 1, [10, 11, 12])
    f.update()
    f.close()

    # Verify the created file
    g = exodusIIcpp.File(file_path, exodusIIcpp.FileAccess.READ)
    g.read()

    assert g.get_dim() == 1
    assert g.get_num_nodes() == 3
    assert g.get_num_elements() == 2
    assert g.get_num_element_blocks() == 1
    assert g.get_num_node_sets() == 0
    assert g.get_num_side_sets() == 0

    g.close()


def test_append_time_step(tmp_dir):
    """Test appending time steps to an existing file."""
    file_path = str(tmp_dir / "append_test.e")

    # Create initial file with one time step
    f = exodusIIcpp.File()
    f.create(file_path)
    assert f.is_opened()
    f.init("test", 2, 3, 1, 1, 0, 0)

    x = [0, 1, 0]
    y = [0, 0, 1]
    f.write_coords(x, y)
    f.write_coord_names()

    connect1 = [1, 2, 3]
    f.write_block(1, "TRI3", 1, connect1)
    blk_names = ["blk1"]
    f.write_block_names(blk_names)

    f.write_time(1, 0.0)

    nv_names = ["nv1"]
    f.write_nodal_var_names(nv_names)
    f.write_nodal_var(1, 1, [1.0, 2.0, 3.0])

    f.update()
    f.close()

    # Verify initial file has 1 time step
    f = exodusIIcpp.File(file_path, exodusIIcpp.FileAccess.READ)
    assert f.is_opened()
    f.read_times()
    assert f.get_num_times() == 1
    f.close()

    # Open with append and add another time step
    f = exodusIIcpp.File(file_path, exodusIIcpp.FileAccess.APPEND)
    assert f.is_opened()
    f.read_times()
    assert f.get_num_times() == 1

    # Write second time step
    next_step = f.get_num_times() + 1
    f.write_time(next_step, 1.0)
    f.write_nodal_var(next_step, 1, [2.0, 4.0, 6.0])

    f.update()
    f.close()

    # Verify file now has 2 time steps
    f = exodusIIcpp.File(file_path, exodusIIcpp.FileAccess.READ)
    assert f.is_opened()
    f.read_times()
    assert f.get_num_times() == 2

    times = f.get_times()
    assert len(times) == 2
    assert times[0] == pytest.approx(0.0)
    assert times[1] == pytest.approx(1.0)

    # Verify variable values for both time steps
    ts1_values = f.get_nodal_variable_values(1, 1)
    assert len(ts1_values) == 3
    assert ts1_values[0] == pytest.approx(1.0)
    assert ts1_values[1] == pytest.approx(2.0)
    assert ts1_values[2] == pytest.approx(3.0)

    ts2_values = f.get_nodal_variable_values(2, 1)
    assert len(ts2_values) == 3
    assert ts2_values[0] == pytest.approx(2.0)
    assert ts2_values[1] == pytest.approx(4.0)
    assert ts2_values[2] == pytest.approx(6.0)

    f.close()


def test_read_square(assets_dir):
    """Test reading and parsing a pre-existing mesh file."""
    f = exodusIIcpp.File(str(assets_dir / "square.e"), exodusIIcpp.FileAccess.READ)

    if f.is_opened():
        blk_names = f.read_block_names()
        assert len(blk_names) == 1
        assert 0 in blk_names
        assert blk_names[0] == ""

        ss_names = f.read_side_set_names()
        assert len(ss_names) == 4
        assert ss_names[0] == "bottom"
        assert ss_names[1] == "right"
        assert ss_names[2] == "top"
        assert ss_names[3] == "left"

        ns_names = f.read_node_set_names()
        assert len(ns_names) == 4
        assert ns_names[0] == "bottom"
        assert ns_names[1] == "right"
        assert ns_names[2] == "top"
        assert ns_names[3] == "left"

        f.read_times()
        times = f.get_times()
        assert len(times) == 2
        assert times[0] == 0.0
        assert times[1] == 1.0

        nodal_var_names = f.get_nodal_variable_names()
        assert len(nodal_var_names) == 1
        assert nodal_var_names[0] == "u"

        elem_var_names = f.get_elemental_variable_names()
        assert len(elem_var_names) == 0

        global_var_names = f.get_global_variable_names()
        assert len(global_var_names) == 0

        var_idx = 1
        u_var_0 = f.get_nodal_variable_values(1, var_idx)
        assert len(u_var_0) == 9
        for val in u_var_0:
            assert val == pytest.approx(0.0)

        u_var_1 = f.get_nodal_variable_values(2, var_idx)
        assert len(u_var_1) == 9
        expected_vals = [0.0, 0.5, 0.5, 0.0, 1.0, 1.0, 0.5, 0.0, 1.0]
        for i, expected in enumerate(expected_vals):
            assert u_var_1[i] == pytest.approx(expected)

        f.close()


def test_custom_coord_names(tmp_dir):
    """Test writing and reading custom coordinate names."""
    file_path = str(tmp_dir / "coords.e")

    f = exodusIIcpp.File()
    f.create(file_path)
    assert f.is_opened()
    f.init("test", 2, 3, 0, 0, 0, 0)

    x = [0, 1, 0]
    y = [0, 0, 1]
    f.write_coords(x, y)
    f.write_coord_names(["r", "z"])
    f.close()

    # Verify the created file
    g = exodusIIcpp.File(file_path, exodusIIcpp.FileAccess.READ)
    g.read()
    coord_names = g.get_coord_names()
    assert len(coord_names) == 2
    assert coord_names[0] == "r"
    assert coord_names[1] == "z"
    g.close()


def test_test(assets_dir):
    """Test reading test.exo file with global and elemental variables."""
    f = exodusIIcpp.File(str(assets_dir / "test.exo"), exodusIIcpp.FileAccess.READ)

    if f.is_opened():
        global_var_names = f.get_global_variable_names()
        assert len(global_var_names) == 1
        assert global_var_names[0] == "glo_vars"

        vals = f.get_global_variable_values(1)
        assert len(vals) == 1
        assert vals[0] == pytest.approx(0.02)

        vals = f.get_global_variable_values(5)
        assert len(vals) == 1
        assert vals[0] == pytest.approx(0.1)

        vals = f.get_global_variable_values(10)
        assert len(vals) == 1
        assert vals[0] == pytest.approx(0.2)

        vals = f.get_global_variable_values(1, 1)
        assert len(vals) == 10
        expected = [0.02, 0.04, 0.06, 0.08, 0.1, 0.12, 0.14, 0.16, 0.18, 0.2]
        for i, expected_val in enumerate(expected):
            assert vals[i] == pytest.approx(expected_val)

        vals = f.get_global_variable_values(1, 4, 7)
        assert len(vals) == 4
        expected = [0.08, 0.10, 0.12, 0.14]
        for i, expected_val in enumerate(expected):
            assert vals[i] == pytest.approx(expected_val)

        elem_var_names = f.get_elemental_variable_names()
        assert len(elem_var_names) == 3
        assert elem_var_names[0] == "ele_var0"
        assert elem_var_names[1] == "ele_var1"
        assert elem_var_names[2] == "ele_var2"

        ev3b2_vals = f.get_elemental_variable_values(10, 3, 11)
        assert len(ev3b2_vals) == 1
        assert ev3b2_vals[0] == pytest.approx(7.1)

        ss_cnts, ss_nodes = f.get_side_set_node_list(31)
        assert len(ss_cnts) == 2
        assert ss_cnts[0] == 2
        assert ss_cnts[1] == 2
        assert len(ss_nodes) == 4
        assert ss_nodes == [2, 3, 7, 8]

        f.close()
