#pragma once

#include "fmt/printf.h"
#include "fmt/color.h"

namespace internal {

template <typename... T>
void
error_print(fmt::format_string<T...> format, T... args)
{
    fmt::print(stderr, "[ERROR] ");
    fmt::print(stderr, format, std::forward<T>(args)...);
    fmt::print(stderr, "\n");
}

/// Terminate the run
[[noreturn]] inline void
terminate(int status = 1)
{
    exit(status);
}

} // namespace internal

template <typename... T>
[[noreturn]] void
error(fmt::format_string<T...> format, T... args)
{
    internal::error_print(format, std::forward<T>(args)...);
    internal::terminate();
}
