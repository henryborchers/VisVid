FROM ubuntu:20.04 as base_image

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
        python3-venv \
        ffmpeg


ENV SONAR_SCANNER_OPTS="-server"
ENV CONAN_NON_INTERACTIVE=1
ARG PIP_EXTRA_INDEX_URL
ARG PIP_TRUSTED_HOST
ARG PIP_INDEX_URL

#==============================================================================
FROM base_image as SONAR_BUILDER
RUN apt-get update && \
    apt-get install -y unzip
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
    ln -s /opt/sonar/sonar-scanner-$SONAR_SCANNER_VERSION-linux/bin/sonar-scanner /usr/local/bin/sonar-scanner && \
    which sonar-scanner


#==============================================================================
#libalsa/1.2.4
FROM base_image as CONAN_BUILDER
ARG USER_ID=1000
ARG GROUP_ID=1000
RUN groupadd -g $GROUP_ID user && \
    useradd -u $USER_ID -s /bin/sh -m -d /home/user -g user user && \
    mkdir -p /.cache/pip && chgrp -R  user /.cache/ && \
    chmod -R g+w /.cache/
RUN python3 -m pip install conan cmake

COPY conanfile.py /tmp/
ENV CONAN_USER_HOME=/home/user
USER user
WORKDIR /home/user
COPY --chown=user:user ci/dockerfiles/linux/20.04/conan/profile.ini  /home/user/.conan/profiles/default
COPY --chown=user:user ci/dockerfiles/conan/remotes.json  /home/user/.conan/remotes.json
RUN conan install /tmp/ --no-imports --build missing -o with_createVisuals=True --update && \
    conan remove --locks

#==============================================================================
FROM base_image as PYTHON_BASE
RUN apt update && \
    apt-get install -y software-properties-common && \
    add-apt-repository ppa:deadsnakes/ppa && \
    apt update

#==============================================================================
FROM PYTHON_BASE as WHEEL_BUILDER
RUN python3 -m pip install --upgrade pip && \
    python3 -m pip install wheel
COPY requirements-dev.txt /tmp/requirements-dev.txt
COPY ci/dockerfiles/python/shared/requirements-ci.txt /tmp/requirements-ci.txt
RUN python3 -m pip wheel -r /tmp/requirements-dev.txt -r /tmp/requirements-ci.txt -w /wheels

#==============================================================================
FROM base_image as DR_MEMORY_BUILDER
RUN apt install -y wget
WORKDIR /drmemory
RUN wget --no-verbose https://github.com/DynamoRIO/drmemory/releases/download/release_2.3.0/DrMemory-Linux-2.3.0-1.tar.gz && \
    mkdir -p /opt/drmemory && tar xzf DrMemory-Linux-2.3.0-1.tar.gz -C /opt/drmemory --strip-components=1 && \
    ln -s /opt/drmemory/bin64/drmemory /usr/local/bin/drmemory && \
    drmemory -version


#==============================================================================
# FINAL IMAGE

FROM PYTHON_BASE

ARG USER_ID=1000
ARG GROUP_ID=1000
RUN groupadd -g $GROUP_ID user && \
    useradd -u $USER_ID -s /bin/sh -m -d /home/user -g user user && \
    mkdir -p /.cache/pip && chgrp -R  user /.cache/ && \
    chmod -R g+w /.cache/

COPY --from=SONAR_BUILDER /opt/sonar /opt/sonar/
RUN ln -s /opt/sonar/build-wrapper-linux-x86/build-wrapper-linux-x86-64 /usr/local/bin/build-wrapper-linux-x86-64 && \
    which build-wrapper-linux-x86-64
ENV SONAR_SCANNER_VERSION=4.4.0.2170
ARG SONAR_SCANNER_HOME=/opt/sonar/${SONAR_SCANNER_VERSION}-linux
RUN ln -s /opt/sonar/sonar-scanner-${SONAR_SCANNER_VERSION}-linux/bin/sonar-scanner /usr/local/bin/sonar-scanner && \
    which sonar-scanner
RUN mkdir -p /home/user/.sonar/ && chmod -R 777 /home/user/
COPY --from=WHEEL_BUILDER /wheels /wheels/
COPY ci/dockerfiles/python/shared/requirements-ci.txt /tmp/requirements-ci.txt
RUN python3 -m pip install --upgrade pip && \
    python3 -m pip install -r /tmp/requirements-ci.txt && \
    python3 -m pip install cmake && \
    python3 -m pip install conan gcovr

COPY --from=DR_MEMORY_BUILDER /opt/drmemory /opt/drmemory/
RUN ln -s /opt/drmemory/bin64/drmemory /usr/local/bin/drmemory && \
    drmemory -version

COPY conanfile.py /tmp/
ENV CONAN_USER_HOME=/home/user
RUN apt update && apt install -y ffmpeg
USER user
WORKDIR /tmp
COPY --from=CONAN_BUILDER --chown=user:user /home/user/.conan /home/user/.conan/
RUN conan install /tmp/ --no-imports --build missing -o with_createVisuals=True --update && \
    chown -R user /home/user/.conan/data
