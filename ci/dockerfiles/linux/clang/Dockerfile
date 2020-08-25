FROM ubuntu:20.04

ARG USER_ID=1000
ARG GROUP_ID=1000
RUN groupadd -g $GROUP_ID user && \
    useradd -u $USER_ID -s /bin/sh -m -d /home/user -g user user

COPY ci/dockerfiles/linux/clang/packages.txt /packages/packages.txt
RUN apt-get update && \
    xargs -a /packages/packages.txt apt install -y  && \
    rm -rf /var/lib/apt/lists/*
RUN python3 -m pip install pip --upgrade && pip install cmake conan
