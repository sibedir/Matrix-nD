#pragma once

#include <exception>
#include <string>

namespace sib {

    std::string ExceptionInfo(const std::exception& exc);

    std::string ExceptionAsString(const std::exception& exc);

}