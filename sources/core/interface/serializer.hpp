#pragma once

#include <common/event/normalized_event.hpp>
#include <core/error_handling/error_handling.hpp>
#include <cstdint>
#include <functional>
#include <span>
#include <vector>

namespace core::interface {
class ISerializer {
public:
    using OnSuccess = std::function<void(const std::vector<common::event::NormalizedEvent>&)>;
    using OnFail = std::function<void(core::error_handling::ErrorCode)>;

    virtual void Serialize(std::span<std::byte> data, OnSuccess successed, OnFail failed) = 0;
    virtual ~ISerializer() = default;

protected:
    static constexpr uint64_t INVALID_UPDATE_ID = 0;
    uint64_t m_lastUpdateId{INVALID_UPDATE_ID};
};
}  // namespace core::interface
