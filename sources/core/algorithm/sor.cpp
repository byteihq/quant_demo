#include "sor.hpp"

#include <Eigen/Dense>
#include <core/log/log.hpp>

namespace core::algorithm {
static inline float ComputeVarCost(float gammaTemp, float phiPerm, float volume) {
    return (gammaTemp * gammaTemp + phiPerm * phiPerm) * volume * volume;
}

void SOR::Compute(std::vector<VenueData>& venues) const {
    size_t N = venues.size();
    if (N == 0) {
        return;
    }

    Eigen::VectorXd expected_cost(N);
    Eigen::VectorXd var_cost(N);
    for (size_t i = 0; i < N; i++) {
        float mid = (venues[i].vwapBid + venues[i].vwapAsk) / 2.0;
        expected_cost(i) = mid;
        var_cost(i) = ComputeVarCost(venues[i].gammaTemp, venues[i].phiPerm, m_targetAmount);
    }

    Eigen::VectorXd w(N);
    float sumInv = 0;
    for (size_t i = 0; i < N; i++)
        sumInv += 1.0 / (expected_cost(i) + m_lambda * var_cost(i));
    for (size_t i = 0; i < N; i++)
        w(i) = 1.0 / (expected_cost(i) + m_lambda * var_cost(i)) / sumInv;

    for (size_t i = 0; i < N; i++) {
        float vol = w(i) * m_targetAmount;
        LOG(info, "Order result: name={}, volume={}, E[cost]={}, Var(cost)={}", venues[i].name, vol,
            expected_cost(i), var_cost(i));
    }
}
}  // namespace core::algorithm
