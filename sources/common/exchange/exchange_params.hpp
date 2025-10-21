#pragma once

namespace common::exchange {

/**
 * @brief Parameters defining exchange-specific configuration.
 *
 * This structure holds configurable parameters used for trading or
 * market data normalization on a specific exchange.
 */
struct ExchangeParams {
    float takerFee;     /**< The taker fee rate applied to executed trades. */
    float lambda;       /**< A scaling or decay parameter used in computations (e.g., exponential
                           smoothing). */
    float targetAmount; /**< The target trade amount or position size. */
};

}  // namespace common::exchange
