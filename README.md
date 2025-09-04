## Toolchain
- C++20
- Cmake
- Conan

## 3rd party
- boost/1.85.0 - networking
- openssl/3.1.4 - ssl handshake
- spdlog/1.15.3 - logging
- simdjson/3.13.0 - parsing data from exchange
- eigen/3.4.0 - math
- magic_enum/0.9.7 - pretty print

## Build
### Docker (repro build)
```sh
cd sources
docker build -t quant_demo .
```
### Local
Conan 2.0 required
```sh
cmake -S sources -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Example
Logs on real data
```
[18:01:43.706] [info] [handler.cpp:47] Created parser for target: /ws/ethusdt@depth20@100ms - Depth. Id: 0
[18:01:43.708] [info] [handler.cpp:47] Created parser for target: /ws/ethusdt@trade@50ms - Trade. Id: 1
[18:01:43.804] [info] [sor.cpp:34] Order result: volume=binance, E[cost]=0.9999654, Var(cost)=4339.4150390625
[18:01:43.804] [info] [sor.cpp:34] Order result: volume=binance, E[cost]=1.0000346, Var(cost)=4339.11474609375
[18:01:43.902] [info] [sor.cpp:34] Order result: volume=binance, E[cost]=0.6666538, Var(cost)=4339.3857421875
```
