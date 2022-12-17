FROM ubuntu:22.04 

WORKDIR /usr/src/app 

ENV DEBIAN_FRONTEND="noninteractive"

RUN apt-get -qq update
RUN apt-get -qq install gcc g++ build-essential yasm tar zip unzip p7zip-full python3 python3-pip curl xz-utils software-properties-common lsb-release wget apt-utils git
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null

RUN apt-add-repository -y "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main" 
RUN apt-get -qq update 
RUN apt install kitware-archive-keyring
RUN rm /etc/apt/trusted.gpg.d/kitware.gpg
RUN apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 6AF7F09730B3F0A4
RUN apt-get -qq update && apt-get install -qq cmake


RUN apt-get -qq install libcrypto++-dev libssl-dev libc-ares-dev libcurl4-openssl-dev libsqlite3-dev libsodium-dev libraw-dev zlib1g-dev ninja-build libboost-all-dev pkg-config autoconf libreadline-dev

RUN apt-get install locales 
RUN locale-gen en_US.UTF-8
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US:en
ENV LC_ALL en_US.UTF-8

COPY . . 

ENV VCPKG_FORCE_SYSTEM_BINARIES=1

RUN python3 build.py 

RUN echo "build completed"

CMD ["/usr/src/app/cmake-build-debug/megasdkrestpp"]