#pragma once

#include <cstdint>
#include <string_view>

#include "notifier.hpp"

namespace core::interface {
class ISession {
public:
    virtual void Connect(std::string_view source, std::string_view target, uint16_t port,
                         INotifier* notifier) noexcept = 0;
    virtual ~ISession() = default;
};
}  // namespace core::interface