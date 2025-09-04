#pragma once

#include <fmt/format.h>

#include <string>

namespace core::error_handling {
enum ErrorCode : int32_t {
    eUnexpected,
    // network
    eResolveFailed,
    eConnectionFailed,
    eSslHandshakeFailed,
    eHandshakeFailed,
    eReadFailed,
    eMsgTooBig,
    // serializer
    eInvalidJson,
    eDataGap,
    eDataDuplicate,

};

std::string format_as(ErrorCode ec);
}  // namespace core::error_handling
