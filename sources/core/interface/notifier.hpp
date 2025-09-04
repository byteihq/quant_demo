#pragma once

#include <core/error_handling/error_handling.hpp>
#include <functional>
#include <span>

namespace core::interface {
class INotifier {
public:
    std::function<void()> OnConnectionSeccessed;
    std::function<void(core::error_handling::ErrorCode)> OnConnectionFailed;
    std::function<void(std::span<std::byte>)> OnReceiveSuccessed;
    std::function<void(core::error_handling::ErrorCode)> OnReceiveFailed;
    std::function<bool()> OnStopRequested;
    std::function<void()> OnStop;

    virtual ~INotifier() = default;


};
}  // namespace core::interface
