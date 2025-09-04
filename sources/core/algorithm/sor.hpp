#pragma once

#include <string_view>
#include <vector>

namespace core::algorithm {
struct VenueData {
    std::string_view name;
    float vwapBid;
    float vwapAsk;
    float gammaTemp;
    float phiPerm;
};

class SOR final {
public:
    SOR(float lambda, float target) : m_lambda(lambda), m_targetAmount(target) {}
    void Compute(std::vector<VenueData>& venues) const;

private:
    float m_lambda;
    float m_targetAmount;
};
}  // namespace core::algorithm
