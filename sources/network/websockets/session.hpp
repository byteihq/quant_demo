#pragma once

#include <boost/asio/io_context.hpp>
#include <core/interface/session.hpp>
#include <cstdint>
#include <memory>
#include <string_view>

namespace network::websockets {
class Session final : public core::interface::ISession {
public:
    explicit Session(boost::asio::io_context& ioc);

    void Connect(std::string_view source, std::string_view target, uint16_t port,
                 core::interface::INotifier* notifier) noexcept override;

    ~Session();

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};
}  // namespace network::websockets
