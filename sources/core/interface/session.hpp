#pragma once

#include <cstdint>
#include <string_view>

#include "notifier.hpp"

namespace core::interface {

/**
 * @brief Interface for a network or exchange session.
 *
 * The ISession interface defines the contract for establishing and managing
 * connections to a data source or trading venue. Implementations handle
 * protocol-level communication and event dispatch via an INotifier.
 */
class ISession {
public:
    /**
     * @brief Establishes a connection to the specified source and target.
     *
     * Implementations should attempt to connect to the endpoint identified
     * by the source, target, and port, and use the provided notifier to
     * report success, failure, and incoming events.
     *
     * @param source The network source or host name.
     * @param target The target symbol, channel, or path for the session.
     * @param port The network port to connect to.
     * @param notifier Pointer to an INotifier instance for callbacks.
     */
    virtual void Connect(std::string_view source, std::string_view target, uint16_t port,
                         INotifier* notifier) noexcept = 0;

    /**
     * @brief Virtual destructor for proper cleanup in derived classes.
     */
    virtual ~ISession() = default;
};

}  // namespace core::interface
