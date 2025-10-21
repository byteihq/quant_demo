#pragma once

#include <common/event/normalized_event.hpp>
#include <vector>

namespace core::algorithm {

/**
 * @brief Represents a single data point used in the Almgren–Chriss model.
 *
 * Each data point captures market microstructure dynamics such as mid-price changes
 * and signed trading volume.
 */
struct ACDataPoint {
    float delta_mid;         /**< Change in mid-price between consecutive events. */
    float signed_volume;     /**< Signed trade volume (+ for buy, − for sell). */
    float cum_signed_volume; /**< Cumulative signed trade volume up to this point. */
};

/**
 * @brief Result of the Almgren–Chriss regression computation.
 *
 * Contains parameters that describe temporary and permanent market impacts.
 */
struct ACResult {
    double gammaTemp; /**< Temporary impact coefficient (γ_temp). */
    double phiPerm;   /**< Permanent impact coefficient (φ_perm). */
};

/**
 * @brief Tracks market events and computes Almgren–Chriss model parameters.
 *
 * The class collects normalized market events, derives microstructure data points,
 * and performs regression analysis to estimate impact coefficients.
 */
class AlmgrenChrissTracker final {
public:
    /**
     * @brief Adds a new normalized market event for processing.
     *
     * @param e The normalized event to be added.
     */
    void AddEvent(const common::event::NormalizedEvent& e);

    /**
     * @brief Clears all stored data points and resets internal state.
     */
    inline void ClearEvents() { m_data.clear(); }

    /**
     * @brief Returns the collected data points used for regression.
     *
     * @return Const reference to the vector of data points.
     */
    inline const std::vector<ACDataPoint>& GetData() const noexcept { return m_data; }

    /**
     * @brief Computes the Almgren–Chriss regression on the collected data.
     *
     * @return The regression result containing temporary and permanent impact coefficients.
     */
    ACResult ComputeRegression() const;

private:
    std::vector<ACDataPoint> m_data; /**< Collected data points for regression. */

    float m_bestBid = 0;      /**< Current best bid price. */
    float m_bestAsk = 0;      /**< Current best ask price. */
    float m_midPrice = 0;     /**< Current mid-price (average of bid and ask). */
    float m_lastMid = 0;      /**< Previous mid-price used for delta computation. */
    float m_cumSignedVol = 0; /**< Cumulative signed trade volume. */

    bool m_initialized = false; /**< Indicates whether the tracker has received initial data. */
};

}  // namespace core::algorithm
