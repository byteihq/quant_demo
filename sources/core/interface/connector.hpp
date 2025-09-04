#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "notifier.hpp"
#include "session.hpp"

namespace core::interface {
class IConnector {
public:
    virtual void Subscribe(std::string_view target, INotifier* notifier) = 0;
    virtual ~IConnector() = default;

protected:
    struct Handler {
        std::unique_ptr<ISession> session;
        INotifier* notifier;
    };

    std::vector<Handler> m_handlers;
};
}  // namespace core::interface
