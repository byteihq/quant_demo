#include "websocket.hpp"

#include <core/error_handling/error_handling.hpp>
#include <core/log/log.hpp>
#include <span>

namespace network::websockets {

namespace ceh = core::error_handling;

Websocket::Websocket(net::io_context& ioc)
    : m_resolver(net::make_strand(ioc)),
      m_ssl(ssl::context::sslv23_client),
      m_ws(net::make_strand(ioc), m_ssl) {
    m_storage.reserve(maxMsgSize);
}

void Websocket::Run(std::string_view source, std::string_view target, uint16_t port) {
    m_host = source;
    m_target = target;
    auto sport = std::to_string(port);
    m_resolver.async_resolve(m_host, sport,
                             beast::bind_front_handler(&Websocket::OnResolve, shared_from_this()));
}

void Websocket::OnResolve(beast::error_code ec, tcp::resolver::results_type results) {
    if (ec) {
        LOG(err, "Failed to resolve target {}/{}. Ec: {} -> {}", m_host, m_target, ec.value(),
            ec.message());
        m_notifier->OnConnectionFailed(ceh::ErrorCode::eConnectionFailed);
        return;
    }

    if (!SSL_set_tlsext_host_name(m_ws.next_layer().native_handle(), std::string(m_host).c_str())) {
        ec = beast::error_code(static_cast<int>(::ERR_get_error()), net::error::get_ssl_category());
        LOG(err, "Failed to set SNI for target {}/{}. Ec: {} -> {}", m_host, m_target, ec.value(),
            ec.message());
        m_notifier->OnConnectionFailed(ceh::ErrorCode::eConnectionFailed);
        return;
    }

    beast::get_lowest_layer(m_ws).async_connect(
        results, beast::bind_front_handler(&Websocket::OnConnect, shared_from_this()));
}

void Websocket::OnConnect(beast::error_code ec, tcp::resolver::results_type::endpoint_type) {
    if (ec) {
        LOG(err, "Failed to connect to target {}/{}. Ec: {} -> {}", m_host, m_target, ec.value(),
            ec.message());
        m_notifier->OnConnectionFailed(ceh::ErrorCode::eConnectionFailed);
        return;
    }

    m_ws.control_callback([this](boost::beast::websocket::frame_type kind,
                                 boost::beast::string_view payload) mutable {
        std::ignore = kind;
        std::ignore = payload;

        LOG(info, "Inited controll block for target {}/{}", m_host, m_target);

        m_ws.async_pong(boost::beast::websocket::ping_data{}, [](boost::beast::error_code) {});
    });

    m_ws.next_layer().async_handshake(
        ssl::stream_base::client,
        beast::bind_front_handler(&Websocket::OnSslHandshake, shared_from_this()));
}

void Websocket::OnSslHandshake(beast::error_code ec) {
    if (ec) {
        LOG(err, "Failed to complete SSL handshake to target {}/{}. Ec: {} -> {}", m_host, m_target,
            ec.value(), ec.message());
        m_notifier->OnConnectionFailed(ceh::ErrorCode::eSslHandshakeFailed);
        return;
    }

    beast::get_lowest_layer(m_ws).expires_never();

    m_ws.async_handshake(m_host, m_target,
                         beast::bind_front_handler(&Websocket::OnHandshake, shared_from_this()));
}

void Websocket::OnHandshake(beast::error_code ec) {
    if (ec) {
        LOG(err, "Failed to complete handshake to target {}/{}. Ec: {} -> {}", m_host, m_target,
            ec.value(), ec.message());
        m_notifier->OnConnectionFailed(ceh::ErrorCode::eHandshakeFailed);
        return;
    }

    m_ws.async_read(m_buffer, beast::bind_front_handler(&Websocket::OnRead, shared_from_this()));
}

void Websocket::OnRead(beast::error_code ec, std::size_t) {
    if (m_notifier->OnStopRequested()) {
        m_notifier->OnStop();
        return;
    }

    if (ec) [[unlikely]] {
        LOG(err, "Failed to read msg. Ec: {} -> {}", ec.value(), ec.message());
        m_notifier->OnReceiveFailed(ceh::ErrorCode::eReadFailed);
        m_buffer.clear();
        m_ws.async_read(m_buffer,
                        beast::bind_front_handler(&Websocket::OnRead, shared_from_this()));
        return;
    }

    if (m_buffer.size() > maxMsgSize) [[unlikely]] {
        LOG(err, "Msg size is too big. Max expected size: {}, got: {}", maxMsgSize,
            m_buffer.size());
        m_notifier->OnReceiveFailed(ceh::ErrorCode::eMsgTooBig);
        m_buffer.clear();
        m_ws.async_read(m_buffer,
                        beast::bind_front_handler(&Websocket::OnRead, shared_from_this()));
        return;
    }

    std::size_t copied = 0;
    auto const& seq = m_buffer.data();
    for (auto it = boost::asio::buffer_sequence_begin(seq);
         it != boost::asio::buffer_sequence_end(seq); ++it) {
        std::memcpy(m_storage.data() + copied, it->data(), it->size());
        copied += it->size();
    }

    m_notifier->OnReceiveSuccessed(std::span<std::byte>{m_storage.data(), copied});
    m_buffer.clear();
    m_ws.async_read(m_buffer, beast::bind_front_handler(&Websocket::OnRead, shared_from_this()));
}

}  // namespace network::websockets
