#pragma once

#include <exception>
#include <string>

namespace sib {

    namespace exception {

        std::string Info(const std::exception& exc);

        std::string AsString(const std::exception& exc);

    }
}