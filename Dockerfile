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
    wget \
    libsdl2-dev \
    build-essential -y
RUN wget https://cmake.org/files/v3.10/cmake-3.10.2-Linux-x86_64.sh && \
    sh cmake-3.10.2-Linux-x86_64.sh --skip-license
RUN apt-get install git \n
    python3-all-dev
    doxygen
    swig3.0 -y

USER jenkins_node
