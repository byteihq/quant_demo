#pragma once

namespace core::interface {

/**
 * @brief Generic interface for system or component handlers.
 *
 * The IHandler interface defines a minimal contract for initialization
 * routines used by various system components, such as connectors,
 * processors, or data pipelines.
 */
class IHandler {
public:
    /**
     * @brief Initializes the handler.
     *
     * This method should perform any setup, resource allocation,
     * or registration required before the handler starts operation.
     */
    virtual void Init() = 0;

    /**
     * @brief Virtual destructor for proper cleanup in derived classes.
     */
    virtual ~IHandler() = default;
};

}  // namespace core::interface
