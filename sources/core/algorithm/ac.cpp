#include "ac.hpp"

#include <Eigen/Dense>

namespace core::algorithm {
void AlmgrenChrissTracker::AddEvent(const common::event::NormalizedEvent& e) {
    if (e.source == common::event::Source::Depth) {
        if (!m_initialized) {
            m_bestBid = e.type == common::event::Type::Bid ? e.price : m_bestBid;
            m_bestAsk = e.type == common::event::Type::Ask ? e.price : m_bestAsk;
            m_lastMid = (m_bestBid + m_bestAsk) * 0.5;
            m_initialized = true;
        } else {
            if (e.type == common::event::Type::Bid)
                m_bestBid = e.price;
            if (e.type == common::event::Type::Ask)
                m_bestAsk = e.price;
        }
        m_midPrice = (m_bestBid + m_bestAsk) * 0.5;
    } else if (e.source == common::event::Source::Trade && m_initialized) {
        float delta = m_midPrice - m_lastMid;
        float sign = e.type == common::event::Type::Bid ? 1.0 : -1.0;
        float signed_vol = e.size * sign;
        m_cumSignedVol += signed_vol;

        m_data.emplace_back(delta, signed_vol, m_cumSignedVol);

        m_lastMid = m_midPrice;
    }
}

ACResult AlmgrenChrissTracker::ComputeRegression() const {
    if (m_data.empty()) {
        return {};
    }

    size_t N = m_data.size();
    Eigen::MatrixXd X(N, 2);
    Eigen::VectorXd y(N);

    for (size_t i = 0; i < N; ++i) {
        X(i, 0) = m_data[i].signed_volume;
        X(i, 1) = m_data[i].cum_signed_volume;
        y(i) = m_data[i].delta_mid;
    }

    Eigen::VectorXd beta = (X.transpose() * X).ldlt().solve(X.transpose() * y);

    return ACResult{beta(0), beta(1)};
}
}  // namespace core::algorithm
