#pragma once

#include <common/event/normalized_event.hpp>
#include <common/exchange/exchange_params.hpp>
#include <vector>

namespace core::algorithm {
class VWAP final {
public:
    using event_t = common::event::NormalizedEvent;

    struct Result {
        float percent;
        float vwapBid;
        float vwapAsk;
    };

    inline void AddEvent(const event_t& event) {
        if (event.type == common::event::Type::Unspecified)
            return;
        event.type == common::event::Type::Bid ? m_bids.push_back(event) : m_asks.push_back(event);
    }

    inline void ClearEvents() {
        m_bids.clear();
        m_asks.clear();
    }

    std::vector<VWAP::Result> Compute(float takerFee);

private:
    std::vector<event_t> m_bids;
    std::vector<event_t> m_asks;
};
}  // namespace core::algorithm
