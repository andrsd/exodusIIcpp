# SPDX-FileCopyrightText: 2026 David Andrs <andrsd@gmail.com>
# SPDX-License-Identifier: MIT

import pathlib

import pytest

ASSETS = pathlib.Path(__file__).parent.parent.parent / "test" / "assets"


@pytest.fixture
def assets_dir():
    return ASSETS


@pytest.fixture
def tmp_dir(tmp_path):
    return tmp_path
