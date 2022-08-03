#pragma once

#include "fmt/format.h"

#define EXODUSIICPP_CHECK_ERROR(err) \
    exodusIIcpp::internal::check_error(err, __PRETTY_FUNCTION__, __FILE__, __LINE__)

namespace exodusIIcpp {
namespace internal {

inline void
check_error(int err, const char * func, const char * file, int line)
{
    if (err != 0)
        throw std::runtime_error(fmt::sprintf("Error in %s", func));
}

} // namespace internal
} // namespace exodusIIcpp
