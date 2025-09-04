#include "normalized_event.hpp"

#include <magic_enum/magic_enum.hpp>

namespace common::event {
std::string format_as(const NormalizedEvent& event) {
    return fmt::format(
        "\n\tvenue={}\n\tts={}\n\tprice={}\n\tsize={}\n\tlevel={}\n\ttype={}\n\tsource={}",
        event.venue, event.tsUs, event.price, event.size, event.level,
        magic_enum::enum_name(event.type), magic_enum::enum_name(event.source));
}
}  // namespace common::event
