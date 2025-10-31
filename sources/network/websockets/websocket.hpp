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

/**
 * @brief WebSocket client implementation using Boost.Beast and Asio.
 *
 * The Websocket class manages asynchronous connections to a WebSocket
 * server over TLS/SSL, reads messages, and dispatches events via
 * INotifier callbacks. It supports a maximum message size defined by `maxMsgSize`.
 */
class Websocket final : public std::enable_shared_from_this<Websocket> {
public:
    static constexpr uint16_t maxMsgSize = 10'000; /**< Maximum message size in bytes. */

    /**
     * @brief Constructs a WebSocket client instance.
     *
     * @param ioctx Reference to a Boost.Asio io_context for asynchronous operations.
     */
    explicit Websocket(boost::asio::io_context& ioctx);

    /**
     * @brief Starts the WebSocket connection and handshake process.
     *
     * @param source Hostname or IP of the WebSocket server.
     * @param target WebSocket target path or channel.
     * @param port TCP port to connect to.
     */
    void Run(std::string_view source, std::string_view target, uint16_t port);

    /**
     * @brief Sets the notifier for receiving events.
     *
     * @param notifier Pointer to an INotifier instance.
     */
    inline void SetNotifier(core::interface::INotifier* notifier) noexcept {
        m_notifier = notifier;
    }

    inline void Close() noexcept {
        beast::error_code ec;
        m_ws.close(boost::beast::websocket::close_code::normal, ec);
        std::ignore = ec;
    }

private:
    /**
     * @brief Callback invoked after DNS resolution.
     *
     * @param ec Error code of the resolution.
     * @param results Resolved endpoints.
     */
    void OnResolve(beast::error_code ec, tcp::resolver::results_type results);

    /**
     * @brief Callback invoked after TCP connection is established.
     *
     * @param ec Error code of the connection attempt.
     * @param ep Endpoint connected to.
     */
    void OnConnect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep);

    /**
     * @brief Callback invoked after SSL/TLS handshake is complete.
     *
     * @param ec Error code of the handshake.
     */
    void OnSslHandshake(beast::error_code ec);

    /**
     * @brief Callback invoked after WebSocket handshake is complete.
     *
     * @param ec Error code of the handshake.
     */
    void OnHandshake(beast::error_code ec);

    /**
     * @brief Callback invoked when data is read from the WebSocket.
     *
     * @param ec Error code of the read operation.
     * @param bytes_transferred Number of bytes read.
     */
    void OnRead(beast::error_code ec, std::size_t bytes_transferred);

private:
    tcp::resolver m_resolver; /**< Resolver for DNS lookups. */
    ssl::context m_ssl;       /**< SSL context for TLS connections. */
    beast::websocket::stream<beast::ssl_stream<beast::tcp_stream>> m_ws; /**< WebSocket stream. */
    std::vector<std::byte> m_storage;                /**< Temporary storage buffer. */
    beast::flat_buffer m_buffer;                     /**< Flat buffer for incoming messages. */
    std::string_view m_host;                         /**< WebSocket server hostname. */
    std::string_view m_target;                       /**< WebSocket target path. */
    core::interface::INotifier* m_notifier{nullptr}; /**< Notifier for event callbacks. */
};

}  // namespace network::websockets
