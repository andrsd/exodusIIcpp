#include "exodusIIcppException.h"

namespace exodusIIcpp {

Exception::Exception(const std::string & what_arg) : std::runtime_error(what_arg) {}

Exception::Exception(const char * what_arg) : std::runtime_error(what_arg) {}

} // namespace exodusIIcpp
