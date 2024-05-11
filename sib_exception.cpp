#include "sib_exception.h"

#include <typeinfo>

namespace sib {

    namespace exception {

        static inline constexpr auto _LineBreakIf(const bool line_break) noexcept {
            if (line_break) return "\n";
            return "";
        }

        static inline auto& _NextTab(const bool line_break, std::string& tab) {
            if (!line_break) return tab;
            tab = "   " + tab;
            return tab;
        }

        std::string       _ExceptionInfo(const std::exception& exc, const bool line_break, std::string& tab);
        std::string _NestedExceptionInfo(const std::nested_exception& exc, const bool line_break, std::string& tab);

        static inline std::string _ExceptionInfo(const std::exception& exc, const bool line_break = true) {
            std::string tab = line_break ? "" : " <- ";
            return _ExceptionInfo(exc, line_break, tab);
        }

        std::string _ExceptionInfo(const std::exception& exc, const bool line_break, std::string& tab) {
#ifdef _CPPRTTI
            auto result = tab + typeid(exc).name() + ": \"" + exc.what() + "\"";

            try {
                std::rethrow_if_nested(exc);
            }
            catch (const std::exception& e) {
                result += _LineBreakIf(line_break) + _ExceptionInfo(e, line_break, _NextTab(line_break, tab));
            }
            catch (const std::nested_exception& ne) {
                result += _LineBreakIf(line_break) + _NestedExceptionInfo(ne, line_break, _NextTab(line_break, tab));
            }
            catch (...) {
                result += _LineBreakIf(line_break) + _NextTab(line_break, tab) + "{unknown exception}";
            }

            return result;
#else
            return tab + "exception: \"" + exc.what() + "\"";
#endif
        }

        std::string _NestedExceptionInfo(const std::nested_exception& exc, const bool line_break, std::string& tab) {
            auto result = tab + typeid(exc).name();

            try {
                exc.rethrow_nested();
            }
            catch (const std::exception& e) {
                result += _LineBreakIf(line_break) + _ExceptionInfo(e, line_break, _NextTab(line_break, tab));
            }
            catch (const std::nested_exception& ne) {
                result += _LineBreakIf(line_break) + _NestedExceptionInfo(ne, line_break, _NextTab(line_break, tab));
            }
            catch (...) {
                result += _LineBreakIf(line_break) + _NextTab(line_break, tab) + "{unknown exception}";
            }

            return result;
        }

        std::string Info(const std::exception& exc) {
            return exception::_ExceptionInfo(exc, true);
        }

        std::string exception::AsString(const std::exception& exc) {
            return exception::_ExceptionInfo(exc, false);
        }

    }

}