#include <core/log/log.hpp>
#include <engine/pipeline.hpp>
#include <exchange/binance/handler.hpp>
#include <iostream>

int main() {
    try {
        core::log::init_logger();

        boost::asio::io_context ioc;

        engine::Pipeline pipeline;
        pipeline.AddHandler(std::make_unique<exchange::binance::Handler>(ioc));

        pipeline.Init();

        ioc.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
