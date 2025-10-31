#pragma once

#include <core/error_handling/error_handling.hpp>
#include <functional>
#include <span>

namespace core::interface {

/**
 * @brief Interface for event notification callbacks.
 *
 * The INotifier interface defines a set of callback functions used by
 * network connectors or other asynchronous components to communicate
 * connection, reception, and control events back to the application layer.
 */
class INotifier {
public:
    /**
     * @brief Called when a connection is successfully established.
     */
    std::function<void()> OnConnectionSuccessed;

    /**
     * @brief Called when a connection attempt fails.
     *
     * @param ErrorCode The error code indicating the reason for failure.
     */
    std::function<void(core::error_handling::ErrorCode)> OnConnectionFailed;

    /**
     * @brief Called when data is successfully received.
     *
     * @param span A span containing the received raw byte data.
     */
    std::function<void(std::span<std::byte>)> OnReceiveSuccessed;

    /**
     * @brief Called when data reception fails.
     *
     * @param ErrorCode The error code indicating the reason for failure.
     */
    std::function<void(core::error_handling::ErrorCode)> OnReceiveFailed;

    /**
     * @brief Called to check whether a stop has been requested.
     *
     * @return True if a stop has been requested, false otherwise.
     */
    std::function<bool()> OnStopRequested;

    /**
     * @brief Called when the component or session is stopped.
     */
    std::function<void()> OnStop;

    /**
     * @brief Virtual destructor for safe cleanup in derived classes.
     */
    virtual ~INotifier() = default;
};

}  // namespace core::interface
