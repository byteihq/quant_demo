#include "pipeline.hpp"

#include <core/log/log.hpp>

namespace engine {
void Pipeline::Init() {
    for (auto& handler : m_handlers) {
        handler->Init();
    }
}
}  // namespace engine