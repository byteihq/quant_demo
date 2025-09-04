#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <core/interface/notifier.hpp>
#include <memory>
#include <string_view>
#include <vector>

namespace network::websockets {

namespace beast = boost::beast;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

class Websocket final : public std::enable_shared_from_this<Websocket> {
public:
    static constexpr uint16_t maxMsgSize = 10'000;

public:
    explicit Websocket(boost::asio::io_context& ioctx);

    void Run(std::string_view source, std::string_view target, uint16_t port);
    inline void SetNotifier(core::interface::INotifier* notifier) noexcept {
        m_notifier = notifier;
    }

private:
    void OnResolve(beast::error_code ec, tcp::resolver::results_type results);
    void OnConnect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep);
    void OnSslHandshake(beast::error_code ec);
    void OnHandshake(beast::error_code ec);
    void OnRead(beast::error_code ec, std::size_t bytes_transferred);

private:
    tcp::resolver m_resolver;
    ssl::context m_ssl;
    beast::websocket::stream<beast::ssl_stream<beast::tcp_stream>> m_ws;
    std::vector<std::byte> m_storage;
    beast::flat_buffer m_buffer;
    std::string_view m_host;
    std::string_view m_target;
    core::interface::INotifier* m_notifier{nullptr};
};
}  // namespace network::websockets
