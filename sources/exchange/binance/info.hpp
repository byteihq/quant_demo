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
static constexpr std::array<std::pair<std::string_view, EventType>, 2> targets = {
    std::pair<std::string_view, EventType>{"/ws/ethusdt@depth20@100ms"sv, EventType::Depth},
    std::pair<std::string_view, EventType>{"/ws/ethusdt@trade@50ms"sv, EventType::Trade}};
static constexpr std::string_view venue = "binance";

static constexpr common::exchange::ExchangeParams params{
    .takerFee = 0.0004, .lambda = 0.1, .targetAmount = 2.0};
}  // namespace exchange::binance
