#pragma once

#include <spdlog/spdlog.h>

#define LOG(logLevel, formatString, ...)                                                     \
    do {                                                                                     \
        try {                                                                                \
            if (spdlog::should_log(spdlog::level::logLevel)) [[unlikely]] {                  \
                spdlog::log(spdlog::level::logLevel, "[" __FILE_NAME__ ":{}] " formatString, \
                            __LINE__, ##__VA_ARGS__);                                        \
            }                                                                                \
        } catch (...) {                                                                      \
        }                                                                                    \
    } while (false)

#ifndef NDEBUG
#define DLOG(logLevel, formatString, ...) LOG(logLevel, formatString, ##__VA_ARGS__)
#else
#define DLOG(logLevel, formatString, ...)
#endif

namespace core::log {
void init_logger();
}  // namespace core::log
