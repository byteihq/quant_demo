#include "error_handling.hpp"

#include <magic_enum/magic_enum.hpp>

namespace core::error_handling {
std::string format_as(ErrorCode ec) {
    return fmt::format("{}", magic_enum::enum_name(ec));
}
}  // namespace core::error_handling
