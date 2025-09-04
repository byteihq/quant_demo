#pragma once

#include <core/interface/handler.hpp>
#include <memory>
#include <vector>

namespace engine {
class Pipeline final {
public:
    using Handler = std::unique_ptr<core::interface::IHandler>;

public:
    inline void AddHandler(Handler handler) { m_handlers.push_back(std::move(handler)); }
    void Init();

private:
    std::vector<Handler> m_handlers;
};
}  // namespace engine