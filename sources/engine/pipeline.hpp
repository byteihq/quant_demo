#pragma once

#include <core/interface/handler.hpp>
#include <memory>
#include <vector>

namespace engine {

/**
 * @brief Pipeline for managing and initializing a sequence of handlers.
 *
 * The Pipeline class maintains a collection of IHandler objects, allowing
 * dynamic addition of handlers and providing a single entry point to
 * initialize all registered handlers in order.
 */
class Pipeline final {
public:
    using Handler = std::unique_ptr<core::interface::IHandler>; /**< Alias for a handler pointer. */

    /**
     * @brief Adds a new handler to the pipeline.
     *
     * Ownership of the handler is transferred to the pipeline.
     *
     * @param handler The handler to add.
     */
    inline void AddHandler(Handler handler) { m_handlers.push_back(std::move(handler)); }

    /**
     * @brief Initializes all registered handlers in the pipeline.
     *
     * Calls the Init() method on each handler in the order they were added.
     */
    void Init();

private:
    std::vector<Handler> m_handlers; /**< Collection of pipeline handlers. */
};

}  // namespace engine
