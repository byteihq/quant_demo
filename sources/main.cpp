#include <core/log/log.hpp>
#include <engine/pipeline.hpp>
#include <exchange/binance/handler.hpp>
#include <exchange/binance/info.hpp>
#include <iostream>

int main() {
    try {
        core::log::init_logger();

        boost::asio::io_context ioc;

        auto depthHandler = std::make_unique<exchange::binance::Handler>(ioc);
        depthHandler->AddTarget(exchange::binance::EventType::Depth, "/ws/ethusdt@depth20@100ms");

        auto tradeHandler = std::make_unique<exchange::binance::Handler>(ioc);
        tradeHandler->AddTarget(exchange::binance::EventType::Trade, "/ws/ethusdt@trade@50ms");

        engine::Pipeline pipeline;
        pipeline.AddHandler(std::move(depthHandler));
        pipeline.AddHandler(std::move(tradeHandler));

        pipeline.Init();

        ioc.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
