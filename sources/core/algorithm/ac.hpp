#pragma once

#include <common/event/normalized_event.hpp>
#include <vector>

namespace core::algorithm {
struct ACDataPoint {
    float delta_mid;
    float signed_volume;
    float cum_signed_volume;
};

struct ACResult {
    double gammaTemp;
    double phiPerm;
};

class AlmgrenChrissTracker final {
public:
    void AddEvent(const common::event::NormalizedEvent& e);
    inline void ClearEvents() { m_data.clear(); }
    inline const std::vector<ACDataPoint>& GetData() const noexcept { return m_data; }

    ACResult ComputeRegression() const;

private:
    std::vector<ACDataPoint> m_data;

    float m_bestBid = 0;
    float m_bestAsk = 0;
    float m_midPrice = 0;
    float m_lastMid = 0;
    float m_cumSignedVol = 0;

    bool m_initialized = false;
};
}  // namespace core::algorithm
