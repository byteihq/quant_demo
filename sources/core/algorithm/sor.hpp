#pragma once

#include <string_view>
#include <vector>

namespace core::algorithm {

/**
 * @brief Represents venue-specific market and model parameters.
 *
 * Each instance stores calculated or estimated parameters for a given exchange or venue,
 * including VWAP (Volume-Weighted Average Price) data and impact coefficients.
 */
struct VenueData {
    std::string_view name; /**< Venue or exchange name. */
    float vwapBid;         /**< Volume-weighted average bid price. */
    float vwapAsk;         /**< Volume-weighted average ask price. */
    float gammaTemp;       /**< Temporary impact coefficient (γ_temp). */
    float phiPerm;         /**< Permanent impact coefficient (φ_perm). */
};

/**
 * @brief Smart Order Router (SOR) algorithm.
 *
 * The SOR algorithm distributes trading volume across venues to optimize execution cost
 * based on liquidity, impact coefficients, and target allocation.
 */
class SOR final {
public:
    /**
     * @brief Constructs a Smart Order Router instance.
     *
     * @param lambda Risk aversion or cost sensitivity parameter.
     * @param target Target trade size or total amount to allocate.
     */
    SOR(float lambda, float target) : m_lambda(lambda), m_targetAmount(target) {}

    /**
     * @brief Computes optimal allocation across venues.
     *
     * Uses venue-specific data (VWAP, impact coefficients) to determine
     * the optimal distribution of the target trade amount.
     *
     * @param venues Vector of venues with their market and model parameters.
     */
    void Compute(std::vector<VenueData>& venues) const;

private:
    float m_lambda;       /**< Risk aversion or sensitivity parameter. */
    float m_targetAmount; /**< Target total amount to allocate across venues. */
};

}  // namespace core::algorithm
