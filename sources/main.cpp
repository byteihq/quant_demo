#include <core/log/log.hpp>
#include <engine/pipeline.hpp>
#include <exchange/binance/handler.hpp>
#include <iostream>

int main() {
    try {
        core::log::init_logger();

        boost::asio::io_context ioc;

        auto depthHandler = std::make_unique<exchange::binance::Handler>(ioc);
        depthHandler->AddTarget(EventType::Depth, "/ws/ethusdt@depth20@100ms");

        auto tradeHandler = std::make_unique<exchange::binance::Handler>(ioc);
        tradeHandler->AddTarget(EventType::Trade, "/ws/ethusdt@trade@50ms");

        engine::Pipeline pipeline;
        pipeline.AddHandler(depthHandler);
        pipeline.AddHandler(tradeHandler);

        pipeline.Init();

        ioc.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
