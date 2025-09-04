#pragma once

#include <core/interface/connector.hpp>
#include <network/websockets/session.hpp>
#include <string_view>

namespace exchange::binance {
using namespace std::literals;

class Connector final : public core::interface::IConnector {
public:
    Connector(boost::asio::io_context& ioc);
    void Subscribe(std::string_view target, core::interface::INotifier* notifier) override;

private:
    boost::asio::io_context& m_ioc;
};
}  // namespace exchange::binance