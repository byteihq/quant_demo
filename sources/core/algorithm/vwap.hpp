#pragma once

#include <common/event/normalized_event.hpp>
#include <common/exchange/exchange_params.hpp>
#include <vector>

namespace core::algorithm {

/**
 * @brief Computes VWAP (Volume-Weighted Average Price) metrics for bids and asks.
 *
 * The VWAP class accumulates normalized market events, groups them by side (bid/ask),
 * and calculates VWAP-based statistics that can be used for market analysis
 * or execution optimization.
 */
class VWAP final {
public:
    using event_t = common::event::NormalizedEvent; /**< Alias for the normalized event type. */

    /**
     * @brief Result of the VWAP computation.
     */
    struct Result {
        float percent; /**< Percentage of total traded volume at this price level. */
        float vwapBid; /**< Volume-weighted average bid price. */
        float vwapAsk; /**< Volume-weighted average ask price. */
    };

    /**
     * @brief Adds a normalized event to the VWAP data set.
     *
     * Bid and ask events are separated internally for independent VWAP computation.
     *
     * @param event The normalized market event to be added.
     */
    inline void AddEvent(const event_t& event) {
        if (event.type == common::event::Type::Unspecified)
            return;
        event.type == common::event::Type::Bid ? m_bids.push_back(event) : m_asks.push_back(event);
    }

    /**
     * @brief Clears all stored bid and ask events.
     */
    inline void ClearEvents() {
        m_bids.clear();
        m_asks.clear();
    }

    /**
     * @brief Computes VWAP statistics for the collected events.
     *
     * Applies the taker fee adjustment when calculating effective VWAP values.
     *
     * @param takerFee The taker fee rate applied to executed trades.
     * @return A vector of VWAP computation results for different percentiles or aggregation levels.
     */
    std::vector<VWAP::Result> Compute(float takerFee);

private:
    std::vector<event_t> m_bids; /**< Stored bid-side events. */
    std::vector<event_t> m_asks; /**< Stored ask-side events. */
};

}  // namespace core::algorithm
