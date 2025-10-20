#include "handler.hpp"

#include <core/log/log.hpp>
#include <functional>
#include <magic_enum/magic_enum.hpp>

#include "info.hpp"

namespace exchange::binance {
namespace ceh = core::error_handling;

Handler::Handler(boost::asio::io_context& ioc)
    : m_connector(ioc),
      m_sorLastUpdate(std::chrono::steady_clock::now()),
      m_sor(params.lambda, params.targetAmount) {}

void Handler::AddTarget(EventType evt, std::string_view target) {
    using namespace std::placeholders;

    serializer_t serializer;
    notifier_t notifier;
    const auto idx = m_parsers.size();

    switch (evt) {
        case EventType::Depth:
            serializer = std::make_unique<DepthSerializer>();
            break;
        case EventType::Trade:
            serializer = std::make_unique<TradeSerializer>();
            break;
        default:
            assert(0 && "Unexpected event type");
            break;
    }

    notifier = std::make_unique<BaseNotifier>();
    notifier->OnConnectionSeccessed = std::bind(&Handler::OnConnectionSeccessed, this, idx);
    notifier->OnConnectionFailed = std::bind(&Handler::OnConnectionFailed, this, idx, _1);
    notifier->OnReceiveSuccessed = std::bind(&Handler::OnReceiveSuccessed, this, idx, _1);
    notifier->OnReceiveFailed = std::bind(&Handler::OnReceiveFailed, this, idx, _1);
    notifier->OnStopRequested = std::bind(&Handler::OnStopRequsted, this, idx);
    notifier->OnStop = std::bind(&Handler::OnStop, this, idx);

    m_parsers.emplace_back(target, std::move(notifier), std::move(serializer), 0);
}

void Handler::Init() {
    using namespace std::placeholders;

    for (const auto& parser : m_parsers) {
        LOG(info, "Created parser for target: {}", parser.target);

        m_connector.Subscribe(parser.target, parser.notifier.get());
    }
}

void Handler::OnConnectionSeccessed(size_t idx) {
    LOG(info, "[{}] successfully connected", idx);
}

void Handler::OnConnectionFailed(size_t idx, ceh::ErrorCode ec) {
    LOG(critical, "[{}] failed to connect. Ec: {}", idx, ec);
    throw std::runtime_error{"Failed to establish connection to required resource"};
}

void Handler::OnReceiveSuccessed(size_t idx, std::span<std::byte> data) {
    using event_t = common::event::NormalizedEvent;

    auto& serializer = m_parsers[idx].serializer;

    const auto onSuccess = [this, idx](const std::vector<event_t>& events) {
        core::algorithm::VenueData venueData;
        venueData.name = venue;
        float vwapBid;
        float vwapAsk;

        for (const auto& ne : events) {
            LOG(trace, "[{}] got new normalized event: {}", idx, ne);

            m_vwap.AddEvent(ne);
            m_acTracker.AddEvent(ne);
        }

        const auto vwapData = m_vwap.Compute(params.takerFee);
        if (vwapData.empty()) {
            if (m_venueEvents.empty())
                return;
            vwapBid = m_venueEvents.back().vwapBid;
            vwapAsk = m_venueEvents.back().vwapAsk;
        } else {
            vwapBid = vwapData[2].vwapBid;  // take 5%
            vwapAsk = vwapData[2].vwapAsk;
        }

        const auto acData = m_acTracker.ComputeRegression();

        venueData.vwapBid = vwapBid;
        venueData.vwapAsk = vwapAsk;
        venueData.gammaTemp = acData.gammaTemp;
        venueData.phiPerm = acData.phiPerm;
        m_venueEvents.push_back(std::move(venueData));

        if (ShouldUpdateSor()) {
            LOG(info, "[{}] compute SOR value based on {} events", idx, m_venueEvents.size());
            m_sor.Compute(m_venueEvents);
            m_acTracker.ClearEvents();
            m_vwap.ClearEvents();
            if (!m_venueEvents.empty())
                m_venueEvents.erase(m_venueEvents.begin() + 1, m_venueEvents.end());
            SorUpdated();
        }
    };

    const auto onFail = [this, idx](ceh::ErrorCode ec) {
        LOG(warn, "[{}] failed to seralize data. Ec: {}. Update statistic", idx, ec);
        ++m_parsers[idx].errors;
    };

    serializer->Serialize(data, onSuccess, onFail);
}

void Handler::OnReceiveFailed(size_t idx, ceh::ErrorCode ec) {
    LOG(warn, "[{}] failed to receive data. Ec: {}. Update statistic", idx, ec);
    ++m_parsers[idx].errors;
}

bool Handler::OnStopRequsted(size_t idx) {
    const auto errors = m_parsers[idx].errors;
    LOG(trace, "[{}] check for stop. Errors count: {}", idx, errors);
    return errors >= 30;
}

void Handler::OnStop(size_t idx) {
    LOG(info, "[{}] finished", idx);
}
}  // namespace exchange::binance
