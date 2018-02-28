FROM ubuntu
MAINTAINER Henry Borchers <henryborchers@yahoo.com>

USER root

RUN apt-get update -y && \
    apt-get install \
    ffmpeg \
    cmake \
    libsdl2-dev \
    build-essential -y

USER jenkins_node
