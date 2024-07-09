#include "gmock/gmock.h"
#include "exodusIIcpp/exodusIIcpp.h"

using namespace exodusIIcpp;

TEST(ErrorTest, test)
{
    EXPECT_THROW(exodusIIcpp::internal::check_error(1, "func", "file", 10), Exception);
}

TEST(ErrorTest, macro)
{
    EXPECT_THROW(EXODUSIICPP_CHECK_ERROR(-1), Exception);
}
