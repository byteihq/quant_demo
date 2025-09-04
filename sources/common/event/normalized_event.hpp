#pragma once

#include <fmt/format.h>

#include <cstdint>
#include <string_view>

namespace common::event {
enum class Type { Unspecified, Ask, Bid };

enum class Source { Depth, Trade };

struct NormalizedEvent {
    std::string_view venue;
    uint64_t tsUs;
    float price;
    float size;
    uint16_t level;
    Type type;
    Source source;
};

std::string format_as(const NormalizedEvent& event);
};  // namespace common::event
