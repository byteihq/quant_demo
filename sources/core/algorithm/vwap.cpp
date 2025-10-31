#include "vwap.hpp"

#include <algorithm>
#include <array>
#include <core/log/log.hpp>
#include <vector>

namespace core::algorithm {
std::vector<VWAP::Result> VWAP::Compute(float takerFee) {
    using event_t = common::event::NormalizedEvent;
    static constexpr std::array<float, 3> percents = {0.01, 0.02, 0.05};
    std::vector<VWAP::Result> results;
    results.reserve(3);

    if (m_asks.empty() || m_bids.empty()) {
        return {};
    }

    LOG(trace, "asks count: {}, bids count: {}", m_asks.size(), m_bids.size());

    std::ranges::sort(m_asks, [](event_t& lhs, event_t& rhs) { return lhs.price < rhs.price; });
    std::ranges::sort(m_bids, [](event_t& lhs, event_t& rhs) { return lhs.price > rhs.price; });

    float bestBid = m_bids.front().price;
    float bestAsk = m_asks.front().price;
    float mid = 0.5 * (bestBid + bestAsk);
    LOG(trace, "Best bid: {}, Best ask: {}, mid: {}", bestBid, bestAsk, mid);

    for (auto percent : percents) {
        float lower = mid * (1 - percent);
        float upper = mid * (1 + percent);

        float volBid = 0;
        float sumBid = 0;

        for (const auto& b : m_bids) {
            if (b.price < lower)
                break;
            volBid += b.size;
            sumBid += b.price * b.size;
        }

        float volAsk = 0;
        float sumAsk = 0;
        for (const auto& a : m_asks) {
            if (a.price > upper)
                break;
            volAsk += a.size;
            sumAsk += a.price * a.size;
        }

        results.push_back({percent * 100, volBid > 0 ? (sumBid / volBid) * (1 - takerFee) : 0,
                           volAsk > 0 ? (sumAsk / volAsk) * (1 - takerFee) : 0});
    }

    return results;
}
}  // namespace core::algorithm
