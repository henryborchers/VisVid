# syntax = docker/dockerfile:1.0-experimental
FROM ubuntu:20.04

ENV TZ=America/Chicago
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update && \
    apt-get install -y \
        build-essential \
        python3-pip \
        pkg-config \
        libgl1-mesa-dev \
        libx11-xcb-dev \
        libfontenc-dev \
        libice-dev \
        libsm-dev \
        libxaw7-dev \
        libxcomposite-dev \
        libxcursor-dev \
        libxdamage-dev \
        libxdmcp-dev \
        libxext-dev \
        libxfixes-dev \
        libxft-dev \
        libxi-dev \
        libxinerama-dev \
        libxkbfile-dev \
        libxmu-dev \
        libxmuu-dev \
        libxpm-dev \
        libxrandr-dev \
        libxrender-dev \
        libxres-dev \
        libxt-dev \
        libxtst-dev \
        libxv-dev \
        libxvmc-dev \
        libxxf86dga-dev \
        libxxf86vm-dev \
        libxss-dev \
        xorg-dev \
        libxcb-render-util0-dev \
        libxcb-icccm4-dev \
        libxcb-image0-dev \
        libxcb-keysyms1-dev \
        libxcb-randr0-dev \
        libxcb-shape0-dev \
        libxcb-sync-dev \
        libxcb-xfixes0-dev \
        libxcb-xinerama0-dev \
        libxcb-xkb-dev \
        xkb-data \
        libcap-dev \
        libdbus-1-dev \
        libva-dev \
        libvdpau-dev \
        libgbm-dev \
        libjack-dev \
        libaudio-dev

ENV CONAN_NON_INTERACTIVE=1
RUN  python3 -m pip install --upgrade pip && \
     python3 -m pip install cmake && \
     python3 -m pip install conan

COPY conanfile.py /tmp/
RUN conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan && \
    conan install /tmp/ --no-imports --build missing --update
