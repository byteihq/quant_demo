#pragma once

#include <array>
#include <common/exchange/exchange_params.hpp>
#include <cstdint>
#include <string_view>

#include "serializer.hpp"

namespace exchange::binance {
enum class EventType { Depth, Trade };

using namespace std::string_view_literals;

static constexpr std::string_view host = "data-stream.binance.vision"sv;
static constexpr uint16_t port = 9443;
static constexpr std::string_view venue = "binance";

static constexpr common::exchange::ExchangeParams params{
    .takerFee = 0.0004, .lambda = 0.1, .targetAmount = 2.0};
}  // namespace exchange::binance
