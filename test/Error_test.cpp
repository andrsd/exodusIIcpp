#include "gmock/gmock.h"
#include "exodusIIcpp.h"
#include <stdexcept>

using namespace exodusIIcpp;

TEST(ErrorTest, test)
{
    EXPECT_THROW(exodusIIcpp::internal::check_error(1, "func", "file", 10), std::runtime_error);
}

TEST(ErrorTest, macro)
{
    EXPECT_THROW(EXODUSIICPP_CHECK_ERROR(-1), std::runtime_error);
}
