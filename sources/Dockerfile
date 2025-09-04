FROM debian:bookworm-slim AS builder

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    python3-pip \
    python3-setuptools \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

RUN pip3 install --break-system-packages conan

WORKDIR /src

COPY . .

RUN conan profile detect

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build -j$(nproc)
