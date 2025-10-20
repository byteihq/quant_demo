cmake_minimum_required(VERSION 3.12)

cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0079 NEW)
cmake_policy(SET CMP0075 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conan_toolchain.cmake)
    set(CONAN_INSTALL_PACKAGES "-c tools.system.package_manager:mode=install")
    set(CONAN_SUDO "-c tools.system.package_manager:sudo=True")
    set(CONAN_PACKAGE_MANAGER "-c tools.system.package_manager:tool=apt-get")
    execute_process(COMMAND conan install ${CMAKE_CURRENT_SOURCE_DIR} --output-folder=${CMAKE_CURRENT_BINARY_DIR} --build=missing --settings=build_type=${CMAKE_BUILD_TYPE} -s compiler.cppstd=${CMAKE_CXX_STANDARD} ${CONAN_INSTALL_PACKAGES} ${CONAN_SUDO} ${CONAN_PACKAGE_MANAGER})
endif()
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_BINARY_DIR}/conan_toolchain.cmake)

project(market_demo LANGUAGES CXX)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

add_library(${PROJECT_NAME}-interface INTERFACE)

target_include_directories(${PROJECT_NAME}-interface INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core/interfaces>"
        "$<INSTALL_INTERFACE:include>"
        )

add_library(core STATIC
    core/algorithm/ac.cpp
    core/algorithm/vwap.cpp
    core/algorithm/sor.cpp
    core/error_handling/error_handling.cpp
    core/log/log.cpp
)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
find_package(spdlog REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(magic_enum REQUIRED)
target_link_libraries(core PUBLIC spdlog::spdlog Eigen3::Eigen magic_enum::magic_enum)

add_library(common STATIC
    common/event/normalized_event.cpp
)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(common PUBLIC spdlog::spdlog magic_enum::magic_enum)

add_library(network STATIC
    network/websockets/session.cpp
    network/websockets/websocket.cpp
)
target_include_directories(network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)
target_link_libraries(network PUBLIC
    ${Boost_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    Threads::Threads
    core
)

add_library(exchange STATIC
    exchange/binance/connector.cpp
    exchange/binance/serializer.cpp
    exchange/binance/handler.cpp
)
target_include_directories(exchange PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
find_package(simdjson REQUIRED)
target_link_libraries(exchange PUBLIC
    core
    network
    common
    simdjson::simdjson
)

add_library(engine STATIC
    engine/pipeline.cpp
)
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(engine PUBLIC
    core
    exchange
)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC engine core)
