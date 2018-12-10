FROM ubuntu
MAINTAINER Henry Borchers <henryborchers@yahoo.com>

USER root

ADD https://cmake.org/files/v3.12/cmake-3.12.2-Linux-x86_64.sh /cmake-Linux-x86_64.sh

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
    valgrind \
    wget \
    build-essential \
    doxygen \
    graphviz \
    git \
    tree \
    gcovr \
    swig3.0 -y
#RUN wget https://cmake.org/files/v3.10/cmake-3.10.2-Linux-x86_64.sh && \
#    sh cmake-3.10.2-Linux-x86_64.sh --skip-license

RUN mkdir /opt/cmake
RUN sh /cmake-Linux-x86_64.sh --prefix=/opt/cmake --skip-license
RUN ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake
RUN ln -s /opt/cmake/bin/ctest /usr/local/bin/ctest
RUN wget https://libsdl.org/release/SDL2-2.0.7.tar.gz && \
    tar -xvzf SDL2-2.0.7.tar.gz && \
    cd SDL2-2.0.7 && \
    mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build . --config Release && \
    cmake --build . --config Release --target install && \
    rm -rf SDL2-2.0.7

USER jenkins_node
