FROM ubuntu:18.04 as conan_builder
RUN apt-get update && \
    apt-get install -y software-properties-common wget

RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | apt-key add - && \
    apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main'

RUN apt-get update && apt-get install -y \
    autoconf \
    automake \
    build-essential \
    cmake \
    libasound2-dev:amd64 \
    libaudio-dev:amd64 \
    libcap-dev \
    libdbus-1-dev \
    libegl1-mesa-dev:amd64 \
    libgbm-dev:amd64 \
    libgl1-mesa-dev \
    libjack-dev:amd64 \
    libpulse-dev:amd64 \
    libtool \
    libva-dev:amd64 \
    libvdpau-dev:amd64 \
    libxcb-icccm4-dev \
    libxcb-image0-dev \
    libxcb-keysyms1-dev  \
    libxcb-render-util0-dev \
    libxcb-shm0-dev:amd64 \
    libxcb-xkb-dev \
    libxcb-xinerama0-dev \
    libxcursor-dev:amd64 \
    libxi-dev:amd64 \
    libxinerama-dev:amd64 \
    libxrandr-dev:amd64 \
    libxss-dev:amd64 \
    mesa-common-dev:amd64 \
    python3 \
    python3-pip \
    xorg-dev \
  && rm -rf /var/lib/apt/lists/*
ENV CONAN_NON_INTERACTIVE=1
RUN python3 -m pip install --upgrade pip && python3 -m pip install conan
COPY ci/dockerfiles/conan/remotes.json /home/user/.conan/remotes.json
ENV CONAN_USER_HOME=/home/user/
COPY ci/dockerfiles/conan/profiles/ /home/user/.conan/profiles/
ENV CONAN_DEFAULT_PROFILE_PATH=/home/user/.conan/profiles/x64
ADD conanfile.py /tmp/
RUN conan install /tmp/ --no-imports --build missing

FROM ubuntu:18.04

RUN apt-get update && \
    apt-get install -y software-properties-common wget

RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | apt-key add - && \
    apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main'

RUN apt-get update && apt-get install -y build-essential \
    python3-pip \
    cmake \
    mesa-common-dev:amd64 \
    libgbm-dev:amd64 \
    libasound2-dev:amd64 \
    libjack-dev:amd64 \
    libpulse-dev:amd64 \
    autoconf \
    automake \
    libtool \
    libsdl2-dev \
    libegl1-mesa-dev:amd64 \
    libaudio-dev:amd64 \
    libxcursor-dev:amd64 \
    libxinerama-dev:amd64 \
    libxi-dev:amd64 \
    libxrandr-dev:amd64 \
    libxss-dev:amd64 \
    libvdpau-dev:amd64 \
    libva-dev:amd64 \
    libxcb-shm0-dev:amd64 \
    libcap-dev \
    libavcodec-dev \
    libavdevice-dev \
    libavfilter-dev \
    libavformat-dev \
    libavresample-dev \
    libavutil-dev \
    libswresample-dev \
    libswscale-dev \
    doxygen \
    python3-all-dev \
    python3-venv \
    cppcheck \
    python3 \
    python3-wheel \
    python3.7-venv \
    clang-tidy \
    clang-tidy-7 \
    libclang-common-7-dev \
    gcovr \
    valgrind \
    ffmpeg \
    xorg-dev \
    libxcb-render-util0-dev \
    libxcb-xkb-dev \
    libxcb-icccm4-dev \
    libxcb-image0-dev \
    libxcb-keysyms1-dev  \
    libxcb-xinerama0-dev \
    curl \
    unzip \
  && rm -rf /var/lib/apt/lists/*


ARG USER_ID=1000
ARG GROUP_ID=1000
RUN groupadd -g $GROUP_ID user && \
    useradd -u $USER_ID -s /bin/sh -m -d /home/user -g user user && \
    mkdir -p /.cache/pip && chgrp -R  user /.cache/ && \
    chmod -R g+w /.cache/
#
COPY --from=conan_builder  /home/user/.conan /home/user/.conan/
ENV CONAN_NON_INTERACTIVE=1
ENV CONAN_USER_HOME=/home/user/
COPY ci/dockerfiles/conan/remotes.json /home/user/.conan/remotes.json
COPY ci/dockerfiles/conan/profiles/ /home/user/.conan/profiles/
ENV CONAN_DEFAULT_PROFILE_PATH=/home/user/.conan/profiles/x64
RUN python3 -m pip install --upgrade pip && python3 -m pip install conan
USER user
ENV SONAR_SCANNER_VERSION=4.4.0.2170
ENV SONAR_SCANNER_HOME=/home/user/.sonar/sonar-scanner-$SONAR_SCANNER_VERSION-linux
RUN curl --create-dirs -sSLo $HOME/.sonar/sonar-scanner.zip \
        https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-$SONAR_SCANNER_VERSION-linux.zip && \
    unzip -o $HOME/.sonar/sonar-scanner.zip -d $HOME/.sonar/ && \
    rm $HOME/.sonar/sonar-scanner.zip && \
    mkdir -p /home/user/.sonar/cache && \
    chown user:user /home/user/.sonar/cache
ENV PATH=$SONAR_SCANNER_HOME/bin:$PATH
ENV SONAR_SCANNER_OPTS="-server"
RUN curl --create-dirs -sSLo $HOME/.sonar/build-wrapper-linux-x86.zip https://sonarcloud.io/static/cpp/build-wrapper-linux-x86.zip && \
    unzip -o $HOME/.sonar/build-wrapper-linux-x86.zip -d $HOME/.sonar/
ENV PATH=/home/user/.sonar/build-wrapper-linux-x86:$PATH
RUN which sonar-scanner

