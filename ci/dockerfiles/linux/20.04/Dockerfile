# syntax = docker/dockerfile:1.0-experimental
FROM ubuntu:20.04 as BASE_IMAGE

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
        libaudio-dev \
        valgrind \
        ninja-build \
        curl \
        unzip \
        doxygen \
        graphviz \
        libavcodec-dev \
        libavformat-dev \
        libavdevice-dev \
        libavresample-dev \
        libavutil-dev \
        libswscale-dev \
        libswresample-dev \
        libsdl2-dev \
        libgtk2.0-dev \
        libxcb-util-dev \
        git \
        libxcb-dri3-dev \
        libasound2-dev \
        unzip

ARG USER_ID=1000
ARG GROUP_ID=1000
RUN groupadd -g $GROUP_ID user && \
    useradd -u $USER_ID -s /bin/sh -m -d /home/user -g user user && \
    mkdir -p /.cache/pip && chgrp -R  user /.cache/ && \
    chmod -R g+w /.cache/

ENV SONAR_SCANNER_OPTS="-server"
ENV CONAN_NON_INTERACTIVE=1
ARG PIP_EXTRA_INDEX_URL
ARG PIP_TRUSTED_HOST
ARG PIP_INDEX_URL

#==============================================================================
FROM BASE_IMAGE as SONAR_BUILDER
ENV SONAR_SCANNER_VERSION=4.4.0.2170
ARG SONAR_SCANNER_HOME=/opt/sonar/${SONAR_SCANNER_VERSION}-linux
ENV SONAR_USER_HOME=/opt/sonar/.sonar
RUN curl --create-dirs -sSLo /tmp/build-wrapper-linux-x86.zip \
        https://sonarcloud.io/static/cpp/build-wrapper-linux-x86.zip && \
    unzip -o /tmp/build-wrapper-linux-x86.zip -d /opt/sonar/ && \
    rm /tmp/build-wrapper-linux-x86.zip && \
    mkdir -p $SONAR_USER_HOME/cache && \
    chmod -R 777 "${SONAR_USER_HOME}" &&\
    ln -s /opt/sonar/build-wrapper-linux-x86/build-wrapper-linux-x86-64 /usr/local/bin/build-wrapper-linux-x86-64 && \
    which build-wrapper-linux-x86-64
RUN curl --create-dirs -sSLo /tmp/sonar-scanner.zip \
        https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-$SONAR_SCANNER_VERSION-linux.zip && \
    unzip -o /tmp/sonar-scanner.zip -d /opt/sonar/ && \
    rm /tmp/sonar-scanner.zip && \
    mkdir -p $SONAR_USER_HOME/cache && \
    chmod -R 777 "${SONAR_USER_HOME}" &&\
    mkdir -p /home/user/.sonar/cache && \
    ln -s /opt/sonar/sonar-scanner-$SONAR_SCANNER_VERSION-linux/bin/sonar-scanner /usr/local/bin/sonar-scanner && \
    which sonar-scanner


#==============================================================================
#libalsa/1.2.4
FROM BASE_IMAGE as CONAN_BUILDER
RUN python3 -m pip install conan cmake
COPY conanfile.py /tmp/
ENV CONAN_USER_HOME=/home/user/
COPY --chown=user:user ci/dockerfiles/linux/20.04/conan/profile.ini  /home/user/.conan/profiles/default
RUN conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan && \
    conan install /tmp/ --no-imports --build missing -o with_createVisuals=True --update && \
    chown -R  user /home/user/.conan/data

#==============================================================================
# FINAL IMAGE

FROM BASE_IMAGE


COPY --from=SONAR_BUILDER /opt/sonar /opt/sonar/
RUN ln -s /opt/sonar/build-wrapper-linux-x86/build-wrapper-linux-x86-64 /usr/local/bin/build-wrapper-linux-x86-64 && \
    which build-wrapper-linux-x86-64
ENV SONAR_SCANNER_VERSION=4.4.0.2170
ARG SONAR_SCANNER_HOME=/opt/sonar/${SONAR_SCANNER_VERSION}-linux
RUN ln -s /opt/sonar/sonar-scanner-${SONAR_SCANNER_VERSION}-linux/bin/sonar-scanner /usr/local/bin/sonar-scanner && \
    which sonar-scanner

RUN python3 -m pip install --upgrade pip && \
    python3 -m pip install cmake && \
    python3 -m pip install conan gcovr

COPY conanfile.py /tmp/
ENV CONAN_USER_HOME=/home/user/
COPY --chown=user:user ci/dockerfiles/linux/20.04/conan/profile.ini  /home/user/.conan/profiles/default
COPY --from=CONAN_BUILDER /home/user/.conan /home/user/.conan/
#RUN conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan && \
RUN conan install /tmp/ --no-imports --build missing -o with_createVisuals=True --update && \
    chown -R  user /home/user/.conan/data
