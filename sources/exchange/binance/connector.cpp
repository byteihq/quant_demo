#include "connector.hpp"

#include "info.hpp"

namespace exchange::binance {
Connector::Connector(boost::asio::io_context& ioc) : m_ioc(ioc) {}

void Connector::Subscribe(std::string_view target, core::interface::INotifier* notifier) {
    auto session = std::make_unique<network::websockets::Session>(m_ioc);
    session->Connect(host, target, port, notifier);
    m_handlers.emplace_back(std::move(session), notifier);
}
}  // namespace exchange::binance
