#pragma once

#include <common/event/normalized_event.hpp>
#include <core/interface/serializer.hpp>
#include <span>
#include <vector>

namespace exchange::binance {
class DepthSerializer final : public core::interface::ISerializer {
public:
    void Serialize(std::span<std::byte> buffer, OnSuccess OnSuccessed, OnFail OnFailed) override;
};

class TradeSerializer final : public core::interface::ISerializer {
public:
    void Serialize(std::span<std::byte> buffer, OnSuccess OnSuccessed, OnFail OnFailed) override;
};
}  // namespace exchange::binance
