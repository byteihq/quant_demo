#pragma once

#include <chrono>
#include <common/event/normalized_event.hpp>
#include <core/algorithm/ac.hpp>
#include <core/algorithm/sor.hpp>
#include <core/algorithm/vwap.hpp>
#include <core/error_handling/error_handling.hpp>
#include <core/interface/handler.hpp>
#include <core/interface/notifier.hpp>
#include <cstdint>
#include <memory>
#include <vector>

#include "connector.hpp"
#include "info.hpp"
#include "notifier.hpp"
#include "serializer.hpp"

namespace exchange::binance {
class Handler final : public core::interface::IHandler {
public:
    Handler(boost::asio::io_context& ioc);
    void AddTarget(EventType evt, std::string_view target);
    void Init() override;

private:
    void OnConnectionSeccessed(size_t idx);
    void OnConnectionFailed(size_t idx, core::error_handling::ErrorCode ec);
    void OnReceiveSuccessed(size_t idx, std::span<std::byte> data);
    void OnReceiveFailed(size_t idx, core::error_handling::ErrorCode ec);
    bool OnStopRequsted(size_t idx);
    void OnStop(size_t idx);

private:
    inline bool ShouldUpdateSor() noexcept {
        using namespace std::chrono_literals;
        return std::chrono::steady_clock::now() - m_sorLastUpdate >= 200ms;
    }

    inline void SorUpdated() noexcept { m_sorLastUpdate = std::chrono::steady_clock::now(); }

private:
    using notifier_t = std::unique_ptr<core::interface::INotifier>;
    using serializer_t = std::unique_ptr<core::interface::ISerializer>;

    struct Parser {
        std::string_view target;
        notifier_t notifier;
        serializer_t serializer;
        uint16_t errors;  // simple statistic
    };
    std::vector<Parser> m_parsers;

    Connector m_connector;
    std::vector<common::event::NormalizedEvent> m_events;
    std::vector<core::algorithm::VenueData> m_venueEvents;
    core::algorithm::VWAP m_vwap;
    core::algorithm::AlmgrenChrissTracker m_acTracker;

    std::chrono::steady_clock::time_point m_sorLastUpdate;
    core::algorithm::SOR m_sor;
};
}  // namespace exchange::binance
