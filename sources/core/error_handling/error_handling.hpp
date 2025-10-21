#pragma once

#include <fmt/format.h>

#include <string>

namespace core::error_handling {

/**
 * @brief Enumerates error codes used throughout the system.
 *
 * The error codes cover various categories such as network failures,
 * serialization issues, and unexpected conditions.
 */
enum ErrorCode : int32_t {
    eUnexpected, /**< An unexpected or unknown error occurred. */

    // --- Network-related errors ---
    eResolveFailed,      /**< Failed to resolve the hostname or address. */
    eConnectionFailed,   /**< Unable to establish a network connection. */
    eSslHandshakeFailed, /**< SSL/TLS handshake failed. */
    eHandshakeFailed,    /**< Generic protocol handshake failure. */
    eReadFailed,         /**< Failed to read data from the network. */
    eMsgTooBig,          /**< Received message exceeds allowed size. */

    // --- Serialization and data-related errors ---
    eInvalidJson,   /**< Invalid or malformed JSON data. */
    eDataGap,       /**< Missing data detected (gap in sequence). */
    eDataDuplicate, /**< Duplicate data detected. */
};

/**
 * @brief Converts an error code to a formatted string.
 *
 * @param ec The error code to format.
 * @return A human-readable string describing the error.
 */
std::string format_as(ErrorCode ec);

}  // namespace core::error_handling
