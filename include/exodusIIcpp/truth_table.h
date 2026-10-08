// SPDX-FileCopyrightText: 2026 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <cstring>

namespace exodusIIcpp {

class TruthTable {
public:
    ~TruthTable() { delete[] this->data; }

    /// Get value from row `i` and column `j`
    ///
    /// @param i Row (1-based)
    /// @param j Column (1-based)
    bool
    operator()(int i, int j)
    {
        int ofst = (i - 1) * this->n_cols + (j - 1);
        return this->data[ofst] == 1;
    }

private:
    TruthTable(int rows, int cols) : n_cols(cols)
    {
        std::size_t n_elems = static_cast<std::size_t>(rows) * static_cast<std::size_t>(cols);
        this->data = new int[n_elems];
        std::memset(this->data, 0, n_elems * sizeof(int));
    }

    int n_cols;
    int * data;

    friend class File;
};

} // namespace exodusIIcpp
