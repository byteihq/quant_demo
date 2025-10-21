#pragma once

#include <fmt/format.h>

#include <cstdint>
#include <string_view>

namespace common::event {

/**
 * @brief Represents the event type (order side).
 */
enum class Type {
    Unspecified, /**< Undefined or unknown event type. */
    Ask,         /**< Ask order (sell). */
    Bid          /**< Bid order (buy). */
};

/**
 * @brief Represents the origin of the event.
 */
enum class Source {
    Depth, /**< Event originates from the order book (market depth). */
    Trade  /**< Event originates from trade data. */
};

/**
 * @brief Normalized representation of a market event.
 *
 * This structure provides a unified format for storing market events
 * received from different exchanges or data feeds.
 */
struct NormalizedEvent {
    std::string_view venue; /**< Exchange or data source identifier. */
    uint64_t tsUs;          /**< Event timestamp in microseconds. */
    float price;            /**< Price at which the event occurred. */
    float size;             /**< Size or volume associated with the event. */
    uint16_t level;         /**< Order book level (0 = top of book). */
    Type type;              /**< Event type (Bid or Ask). */
    Source source;          /**< Source of the event (Depth or Trade). */
};

/**
 * @brief Converts a normalized event to a formatted string.
 *
 * @param event The normalized event to format.
 * @return A human-readable string representation of the event.
 */
std::string format_as(const NormalizedEvent& event);

}  // namespace common::event
