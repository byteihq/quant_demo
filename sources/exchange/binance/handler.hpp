#pragma once

#include <chrono>
#include <common/event/normalized_event.hpp>
#include <core/algorithm/ac.hpp>
#include <core/algorithm/sor.hpp>
#include <core/algorithm/vwap.hpp>
#include <core/error_handling/error_handling.hpp>
#include <core/interface/handler.hpp>
#include <core/interface/notifier.hpp>
#include <cstdint>
#include <memory>
#include <vector>

#include "connector.hpp"
#include "info.hpp"
#include "notifier.hpp"
#include "serializer.hpp"

namespace exchange::binance {

/**
 * @brief Handler for Binance market events and trading logic.
 *
 * The Handler class manages subscriptions, event parsing, VWAP computation,
 * Almgren–Chriss tracking, and Smart Order Routing (SOR) updates for Binance.
 * It uses Boost.Asio for asynchronous operations.
 */
class Handler final : public core::interface::IHandler {
public:
    /**
     * @brief Constructs a Binance handler.
     *
     * @param ioc Reference to a Boost.Asio io_context for async operations.
     */
    Handler(boost::asio::io_context& ioc);

    /**
     * @brief Adds a new subscription target for a specific event type.
     *
     * @param evt The event type (e.g., bid, ask, trade).
     * @param target The subscription target, e.g., trading symbol or channel.
     */
    void AddTarget(EventType evt, std::string_view target);

    /**
     * @brief Initializes the handler.
     *
     * Sets up subscriptions, parsers, and internal state. Overrides IHandler::Init().
     */
    void Init() override;

private:
    /**
     * @brief Callback invoked when a connection succeeds.
     *
     * @param idx Index of the parser/connection.
     */
    void OnConnectionSeccessed(size_t idx);

    /**
     * @brief Callback invoked when a connection fails.
     *
     * @param idx Index of the parser/connection.
     * @param ec Error code indicating the failure reason.
     */
    void OnConnectionFailed(size_t idx, core::error_handling::ErrorCode ec);

    /**
     * @brief Callback invoked when data is successfully received.
     *
     * @param idx Index of the parser/connection.
     * @param data Span containing the received byte data.
     */
    void OnReceiveSuccessed(size_t idx, std::span<std::byte> data);

    /**
     * @brief Callback invoked when data reception fails.
     *
     * @param idx Index of the parser/connection.
     * @param ec Error code indicating the failure reason.
     */
    void OnReceiveFailed(size_t idx, core::error_handling::ErrorCode ec);

    /**
     * @brief Checks if a stop has been requested for a connection.
     *
     * @param idx Index of the parser/connection.
     * @return True if a stop was requested; otherwise false.
     */
    bool OnStopRequsted(size_t idx);

    /**
     * @brief Callback invoked when a connection is stopped.
     *
     * @param idx Index of the parser/connection.
     */
    void OnStop(size_t idx);

private:
    /**
     * @brief Checks whether the Smart Order Router (SOR) should be updated.
     *
     * Uses a 200ms minimum interval between updates.
     *
     * @return True if SOR should be updated; false otherwise.
     */
    inline bool ShouldUpdateSor() noexcept {
        using namespace std::chrono_literals;
        return std::chrono::steady_clock::now() - m_sorLastUpdate >= 200ms;
    }

    /**
     * @brief Updates the timestamp for the last SOR update.
     */
    inline void SorUpdated() noexcept { m_sorLastUpdate = std::chrono::steady_clock::now(); }

private:
    using notifier_t = std::unique_ptr<core::interface::INotifier>; /**< Notifier pointer type. */
    using serializer_t =
        std::unique_ptr<core::interface::ISerializer>; /**< Serializer pointer type. */

    /**
     * @brief Parser structure holding subscription, notifier, and serializer info.
     */
    struct Parser {
        std::string_view target; /**< Subscription target (symbol or channel). */
        notifier_t notifier;     /**< Associated notifier for this parser. */
        serializer_t serializer; /**< Associated serializer for this parser. */
        uint16_t errors;         /**< Simple error statistic counter. */
    };

    std::vector<Parser> m_parsers;                        /**< List of active parsers. */
    Connector m_connector;                                /**< Binance connector instance. */
    std::vector<common::event::NormalizedEvent> m_events; /**< Collected normalized events. */
    std::vector<core::algorithm::VenueData>
        m_venueEvents;                                 /**< Venue-specific events for SOR/VWAP. */
    core::algorithm::VWAP m_vwap;                      /**< VWAP calculator. */
    core::algorithm::AlmgrenChrissTracker m_acTracker; /**< Almgren–Chriss model tracker. */

    std::chrono::steady_clock::time_point m_sorLastUpdate; /**< Timestamp of last SOR update. */
    core::algorithm::SOR m_sor;                            /**< Smart Order Router instance. */
};

}  // namespace exchange::binance
