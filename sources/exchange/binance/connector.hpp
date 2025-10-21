#pragma once

#include <core/interface/connector.hpp>
#include <network/websockets/session.hpp>
#include <string_view>

namespace exchange::binance {

/**
 * @brief Connector implementation for Binance exchange.
 *
 * The Connector class manages subscriptions to Binance market data streams
 * and dispatches events via INotifier callbacks. It uses Boost.Asio for
 * asynchronous network operations.
 */
class Connector final : public core::interface::IConnector {
public:
    /**
     * @brief Constructs a Binance connector.
     *
     * @param ioc Reference to an existing Boost.Asio io_context for asynchronous operations.
     */
    Connector(boost::asio::io_context& ioc);

    /**
     * @brief Subscribes to a specific target (symbol or channel) on Binance.
     *
     * @param target The subscription target (e.g., trading symbol or channel).
     * @param notifier Pointer to an INotifier instance for receiving events.
     */
    void Subscribe(std::string_view target, core::interface::INotifier* notifier) override;

private:
    boost::asio::io_context&
        m_ioc; /**< Reference to the Boost.Asio IO context used for async operations. */
};

}  // namespace exchange::binance
