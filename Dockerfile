FROM ubuntu
MAINTAINER Henry Borchers <henryborchers@yahoo.com>

#USER root

ADD https://cmake.org/files/v3.13/cmake-3.13.5-Linux-x86_64.sh /cmake-Linux-x86_64.sh

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
    python3-venv \
    python3 \
    python3-wheel \
    python3.7-venv \
    tox \
    python3-pip \
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
RUN ln -s /opt/cmake/bin/cpack /usr/local/bin/cpack
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

# Needs this otherwise tox will not have permissions to 
# create virtual environments
# Idea from https://issues.jenkins-ci.org/browse/JENKINS-47026?jql=text%20~%20%22virtualenv%22%20ORDER%20BY%20updated%20DESC

ARG USER_ID=1000
ARG GROUP_ID=1000

RUN groupadd -g $GROUP_ID user && \
    useradd -u $USER_ID -s /bin/sh -g user user
# ==========================================================