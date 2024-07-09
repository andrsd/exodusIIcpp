// SPDX-FileCopyrightText: 2022 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once
#if __has_include(<filesystem>)
    #include <filesystem>
    #include <utility>
namespace exodusIIcpp {
namespace fs = std::filesystem;
}
#elif __has_include(<experimental/filesystem>)
    #include <experimental/filesystem>
namespace exodusIIcpp {
namespace fs = std::experimental::filesystem;
}
#else
    #error Could not find includes: <filesystem> or <experimental/filesystem>
#endif
