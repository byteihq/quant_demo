#pragma once

#include <common/event/normalized_event.hpp>
#include <core/error_handling/error_handling.hpp>
#include <cstdint>
#include <functional>
#include <span>
#include <vector>

namespace core::interface {

/**
 * @brief Interface for serializing and deserializing market events.
 *
 * The ISerializer interface provides asynchronous serialization support,
 * invoking callbacks on success or failure of deserialization.
 */
class ISerializer {
public:
    /// Callback type invoked when deserialization succeeds.
    using OnSuccess = std::function<void(const std::vector<common::event::NormalizedEvent>&)>;

    /// Callback type invoked when deserialization fails.
    using OnFail = std::function<void(core::error_handling::ErrorCode)>;

    /**
     * @brief Deserializes raw data into normalized events.
     *
     * Implementations must invoke the appropriate callback depending on
     * whether deserialization succeeds or fails.
     *
     * @param data The raw byte data to deserialize.
     * @param successed Callback invoked with the deserialized events on success.
     * @param failed Callback invoked with an error code on failure.
     */
    virtual void Serialize(std::span<std::byte> data, OnSuccess successed, OnFail failed) = 0;

    /**
     * @brief Virtual destructor for proper cleanup in derived classes.
     */
    virtual ~ISerializer() = default;

protected:
    static constexpr uint64_t INVALID_UPDATE_ID =
        0; /**< Sentinel value for an invalid update ID. */
    uint64_t m_lastUpdateId{
        INVALID_UPDATE_ID}; /**< Tracks the last successfully processed update ID. */
};

}  // namespace core::interface
