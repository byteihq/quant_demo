#pragma once

#include <common/event/normalized_event.hpp>
#include <core/interface/serializer.hpp>
#include <span>
#include <vector>

namespace exchange::binance {

/**
 * @brief Serializer for Binance order book (depth) events.
 *
 * Parses raw byte data from the Binance depth feed and converts it
 * into normalized market events. Invokes callbacks on success or failure.
 */
class DepthSerializer final : public core::interface::ISerializer {
public:
    /**
     * @brief Deserializes raw depth data.
     *
     * @param buffer Raw byte data from the Binance depth feed.
     * @param OnSuccessed Callback invoked with deserialized events on success.
     * @param OnFailed Callback invoked with an error code on failure.
     */
    void Serialize(std::span<std::byte> buffer, OnSuccess OnSuccessed, OnFail OnFailed) override;
};

/**
 * @brief Serializer for Binance trade events.
 *
 * Parses raw byte data from the Binance trade feed and converts it
 * into normalized market events. Invokes callbacks on success or failure.
 */
class TradeSerializer final : public core::interface::ISerializer {
public:
    /**
     * @brief Deserializes raw trade data.
     *
     * @param buffer Raw byte data from the Binance trade feed.
     * @param OnSuccessed Callback invoked with deserialized events on success.
     * @param OnFailed Callback invoked with an error code on failure.
     */
    void Serialize(std::span<std::byte> buffer, OnSuccess OnSuccessed, OnFail OnFailed) override;
};

}  // namespace exchange::binance
