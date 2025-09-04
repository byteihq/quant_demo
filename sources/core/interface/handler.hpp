#pragma once

namespace core::interface {
class IHandler {
public:
    virtual void Init() = 0;
    virtual ~IHandler() = default;
};

}  // namespace core::interface
