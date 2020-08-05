FROM ubuntu:20.04

ARG USER_ID=1000
ARG GROUP_ID=1000
RUN groupadd -g $GROUP_ID user && \
    useradd -u $USER_ID -s /bin/sh -m -d /home/user -g user user

COPY ci/dockerfiles/static_analysis/clang_tidy/packages.txt /packages/packages.txt
RUN apt-get update && \
    apt-get install -y software-properties-common wget && \
    wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | apt-key add - && \
    apt-add-repository 'deb https://apt.kitware.com/ubuntu/ focal main' && \
    apt-get update && \
    xargs -a /packages/packages.txt apt install -y  && \
    rm -rf /var/lib/apt/lists/*
