#include "log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace core::log {
void init_logger() {
    auto console = spdlog::stdout_color_mt("console");
    console->set_pattern("[%H:%M:%S.%e] [%^%l%$] %v");
    spdlog::set_default_logger(console);
    spdlog::set_level(spdlog::level::trace);
}
}  // namespace core::log
