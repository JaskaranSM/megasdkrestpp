FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive
ENV VCPKG_FORCE_SYSTEM_BINARIES=1

WORKDIR /build
COPY . .
RUN apt-get -qq update \
 && apt-get -qq install apt-utils gpg locales lsb-release wget \
 && echo en_US.UTF-8 UTF-8 >> /etc/locale.gen \
 && wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null \
 && echo "deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main" | tee /etc/apt/sources.list.d/kitware.list >/dev/null \
 && apt-get -qq update \
 && rm /usr/share/keyrings/kitware-archive-keyring.gpg \
 && apt-get -qq install kitware-archive-keyring \
 && apt-get -qq install autoconf \
    build-essential \
    cmake \
    curl \
    g++ \
    gcc \
    git \
    libboost-all-dev \
    libc-ares-dev \
    libcrypto++-dev \
    libcurl4-openssl-dev \
    libraw-dev \
    libreadline-dev \
    libsodium-dev \
    libsqlite3-dev \
    libssl-dev \
    ninja-build \
    p7zip-full \
    pkg-config \
    python3 \
    python3-pip \
    software-properties-common \
    tar \
    unzip \
    xz-utils \
    yasm \
    zip \
    zlib1g-dev \
 && rm -rf /var/lib/apt/lists/* \
 && rm -rf /tmp/* \
 && python3 build.py


FROM ubuntu:22.04

WORKDIR /app
COPY --from=builder /build/cmake-build-debug/megasdkrestpp .
RUN apt-get -qq update \
 && apt-get -qq install libc-ares-dev libcurl4 \
 && rm -rf /var/lib/apt/lists/* \
 && rm -rf /tmp/*

CMD [ "./megasdkrestpp" ]
