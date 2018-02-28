FROM ubuntu
MAINTAINER Henry Borchers <henryborchers@yahoo.com>

USER root

RUN apt-get update -y && \
    apt-get install \
    ffmpeg \
    libavcodec-dev \
    libavdevice-dev \
    libavfilter-dev \
    libavformat-dev \
    libavresample-dev \
    libavutil-dev \
    libpostproc-dev \
    libswresample-dev \
    libswscale-dev \
    libgtk2.0-dev \
    python3-all-dev \
    wget \
    build-essential \
    doxygen \
    git \
    swig3.0 -y
RUN wget https://cmake.org/files/v3.10/cmake-3.10.2-Linux-x86_64.sh && \
    sh cmake-3.10.2-Linux-x86_64.sh --skip-license

RUN wget https://libsdl.org/release/SDL2-2.0.7.tar.gz && \
    tar -xvzf SDL2-2.0.7.tar.gz && \
    cd SDL2-2.0.7 && \
    mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build . --config Release && \
    make install

USER jenkins_node
