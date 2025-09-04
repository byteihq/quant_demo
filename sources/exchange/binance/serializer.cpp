#include "serializer.hpp"

#include <simdjson.h>

#include <charconv>
#include <common/event/normalized_event.hpp>
#include <core/error_handling/error_handling.hpp>
#include <core/log/log.hpp>
#include <stdexcept>

#include "info.hpp"

namespace exchange::binance {
std::vector<std::span<std::byte>> SplitJsonObjects(std::span<std::byte> buffer) {
    std::vector<std::span<std::byte>> objects;
    int depth = 0;
    size_t start = 0;

    for (size_t i = 0; i < buffer.size(); ++i) {
        char c = static_cast<char>(buffer[i]);

        if (c == '{') {
            if (depth == 0) {
                start = i;
            }
            depth++;
        }

        if (c == '}') {
            depth--;
            if (depth == 0) {
                objects.emplace_back(buffer.subspan(start, i - start + 1));
            }
        }
    }

    return objects;
}

inline uint64_t NowUs() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
}

void DepthSerializer::Serialize(std::span<std::byte> buffer, OnSuccess OnSuccessed,
                                OnFail OnFailed) {
    using event_t = common::event::NormalizedEvent;
    std::vector<event_t> events;
    event_t e;
    e.venue = venue;
    e.tsUs = NowUs();
    e.source = common::event::Source::Depth;

    simdjson::ondemand::parser parser;
    const auto objs = SplitJsonObjects(buffer);

    for (const auto& el : objs) {
        simdjson::padded_string json(reinterpret_cast<const char*>(el.data()), el.size());

        auto doc = parser.iterate(json);

        if (doc.error()) [[unlikely]] {
            LOG(warn, "Received invalid json");
            OnFailed(core::error_handling::ErrorCode::eInvalidJson);
            return;
        }

        auto obj = doc.get_object();

        uint64_t curLastUpdate = obj["lastUpdateId"].get_uint64().value();
        if (m_lastUpdateId == INVALID_UPDATE_ID) {
            m_lastUpdateId = curLastUpdate;
        } else if (m_lastUpdateId != curLastUpdate) {
            if (m_lastUpdateId > curLastUpdate) {
                LOG(warn, "Received too old data. Expected: {}, got: {}", m_lastUpdateId,
                    curLastUpdate);
                OnFailed(core::error_handling::ErrorCode::eDataDuplicate);
            } else {
                LOG(warn, "Received too new data. Expected: {}, got: {}", m_lastUpdateId,
                    curLastUpdate);
                OnFailed(core::error_handling::ErrorCode::eDataGap);
            }
            m_lastUpdateId = INVALID_UPDATE_ID;
            return;
        }

        uint16_t lvl = 0;
        for (auto b : obj["bids"].get_array().value()) {
            auto row = b.get_array().value();
            auto it = row.begin();

            std::string_view sprice = (*it).get_string().value();
            float price;
            std::from_chars(sprice.data(), sprice.data() + sprice.size(), price);
            ++it;

            std::string_view ssize = (*it).get_string().value();
            float size;
            std::from_chars(ssize.data(), ssize.data() + ssize.size(), size);

            e.type = common::event::Type::Bid;
            e.price = price;
            e.size = size;
            e.level = lvl++;

            events.push_back(e);
        }

        lvl = 0;
        for (auto a : obj["asks"].get_array().value()) {
            auto row = a.get_array().value();
            auto it = row.begin();

            std::string_view sprice = (*it).get_string().value();
            float price;
            std::from_chars(sprice.data(), sprice.data() + sprice.size(), price);
            ++it;

            std::string_view ssize = (*it).get_string().value();
            float size;
            std::from_chars(ssize.data(), ssize.data() + ssize.size(), size);

            e.type = common::event::Type::Ask;
            e.price = price;
            e.size = size;
            e.level = lvl++;

            events.push_back(e);
        }

        OnSuccessed(events);
        ++m_lastUpdateId;
    }
}

void TradeSerializer::Serialize(std::span<std::byte> buffer, OnSuccess OnSuccessed,
                                OnFail OnFailed) {
    using event_t = common::event::NormalizedEvent;
    std::vector<event_t> events;
    event_t e;
    e.venue = venue;
    e.tsUs = NowUs();
    e.source = common::event::Source::Trade;
    e.type = common::event::Type::Unspecified;
    e.level = 0;

    simdjson::ondemand::parser parser;
    const auto objs = SplitJsonObjects(buffer);

    for (const auto& el : objs) {
        simdjson::padded_string json(reinterpret_cast<const char*>(el.data()), el.size());

        auto doc = parser.iterate(json);

        if (doc.error()) [[unlikely]] {
            LOG(warn, "Received invalid json");
            OnFailed(core::error_handling::ErrorCode::eInvalidJson);
            return;
        }

        auto obj = doc.get_object();

        std::string_view sprice = obj["p"].get_string().value();
        float price;
        std::from_chars(sprice.data(), sprice.data() + sprice.size(), price);

        std::string_view ssize = obj["q"].get_string().value();
        float size;
        std::from_chars(ssize.data(), ssize.data() + ssize.size(), size);

        e.price = price;
        e.size = size;
        events.push_back(e);

        OnSuccessed(events);
    }
}
}  // namespace exchange::binance
