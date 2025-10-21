#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "notifier.hpp"
#include "session.hpp"

namespace core::interface {

/**
 * @brief Interface for exchange or data-feed connectors.
 *
 * The IConnector interface defines the basic contract for subscribing to
 * market data streams or trading channels from various sources.
 * Implementations handle connection management and event propagation
 * through notifier objects.
 */
class IConnector {
public:
    /**
     * @brief Subscribes to a specific market data or trading channel.
     *
     * Implementations should establish or reuse a connection session and
     * bind the provided notifier to receive incoming events or updates.
     *
     * @param target The subscription target (e.g., symbol, topic, or channel).
     * @param notifier Pointer to an INotifier instance that handles received events.
     */
    virtual void Subscribe(std::string_view target, INotifier* notifier) = 0;

    /**
     * @brief Virtual destructor for proper cleanup in derived classes.
     */
    virtual ~IConnector() = default;

protected:
    /**
     * @brief Internal structure for managing connection sessions and notifiers.
     */
    struct Handler {
        std::unique_ptr<ISession> session; /**< Session object managing the connection. */
        INotifier* notifier;               /**< Notifier associated with the session. */
    };

    std::vector<Handler> m_handlers; /**< List of active connection handlers. */
};

}  // namespace core::interface
