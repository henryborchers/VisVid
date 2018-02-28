FROM ubuntu
MAINTAINER Henry Borchers <henryborchers@yahoo.com>

USER root

RUN apt-get update -y && \
    apt-get install \
    ffmpeg \
    libsdl2-dev \
    build-essential -y
RUN wget https://cmake.org/files/v3.10/cmake-3.10.2-Linux-x86_64.sh && \
    sh cmake-3.10.2-Linux-x86_64.sh

USER jenkins_node
