#include "session.hpp"

#include "websocket.hpp"

namespace network::websockets {
class Session::Impl {
public:
    explicit Impl(boost::asio::io_context& ioc) : m_ws{std::make_shared<Websocket>(ioc)} {}

    void Connect(std::string_view source, std::string_view target, uint16_t port,
                 core::interface::INotifier* notifier) {
        m_ws->SetNotifier(notifier);
        m_ws->Run(source, target, port);
    }

    inline void Close() noexcept { m_ws->Close(); }

private:
    std::shared_ptr<Websocket> m_ws;
};

Session::Session(boost::asio::io_context& ioc) : m_impl{new Session::Impl(ioc)} {}

void Session::Connect(std::string_view source, std::string_view target, uint16_t port,
                      core::interface::INotifier* notifier) noexcept {
    m_impl->Connect(source, target, port, notifier);
}

Session::~Session() {
    m_impl->Close();
}
}  // namespace network::websockets
