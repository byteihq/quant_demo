#pragma once

#include <boost/asio/io_context.hpp>
#include <core/interface/session.hpp>
#include <cstdint>
#include <memory>
#include <string_view>

namespace network::websockets {

/**
 * @brief WebSocket session implementation.
 *
 * The Session class establishes and manages a WebSocket connection
 * to a given source and target using Boost.Asio. Events and status
 * updates are communicated via an INotifier.
 */
class Session final : public core::interface::ISession {
public:
    /**
     * @brief Constructs a WebSocket session.
     *
     * @param ioc Reference to a Boost.Asio io_context for asynchronous operations.
     */
    explicit Session(boost::asio::io_context& ioc);

    /**
     * @brief Establishes a connection to the specified source and target.
     *
     * Implements the ISession interface. Uses the provided notifier
     * to report connection success, failures, and incoming messages.
     *
     * @param source The network source or host.
     * @param target The subscription target, e.g., symbol or channel.
     * @param port The network port to connect to.
     * @param notifier Pointer to an INotifier instance for event callbacks.
     */
    void Connect(std::string_view source, std::string_view target, uint16_t port,
                 core::interface::INotifier* notifier) noexcept override;

    /**
     * @brief Destructor. Cleans up internal resources.
     */
    ~Session();

private:
    /**
     * @brief Private implementation (PIMPL) to hide internal details.
     */
    class Impl;

    std::unique_ptr<Impl> m_impl; /**< Pointer to the implementation details. */
};

}  // namespace network::websockets
