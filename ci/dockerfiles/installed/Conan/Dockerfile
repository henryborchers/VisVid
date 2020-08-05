FROM ubuntu:18.04

RUN apt-get update && \
    apt-get install -y software-properties-common wget

RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | apt-key add - && \
    apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main'

RUN apt-get update && apt-get install -y build-essential \
    python3-pip \
    cmake \
    libpulse-dev \
    libva-dev \
    libvdpau-dev \
    libltdl-dev \
    libcap-dev \
    libgbm-dev \
    libjack-dev \
    libaudio-dev \
    git

ENV CONAN_NON_INTERACTIVE=1
ARG USER_ID=1000
ARG GROUP_ID=1000
RUN python3 -m pip install --upgrade pip && python3 -m pip install conan

RUN groupadd -g $GROUP_ID user && \
    useradd -u $USER_ID -s /bin/sh -m -d /home/user -g user user

ENV CONAN_USER_HOME=/home/user/
COPY ci/dockerfiles/conan/remotes.json /home/user/.conan/remotes.json

COPY ci/dockerfiles/conan/profiles/ /home/user/.conan/profiles/
ENV CONAN_DEFAULT_PROFILE_PATH=/home/user/.conan/profiles/x64

ADD conanfile.py /tmp/
RUN conan install /tmp/ --no-imports --build sdl2 --build ffmpeg && conan remove --locks -f
COPY . /workspace/
#libgtk2.0-dev libglib2.0-dev
RUN conan install /workspace -if /build
RUN cmake -S /workspace -B /build -DCMAKE_TOOLCHAIN_FILE:FILEPATH=/build/conan_paths.cmake -DVISVID_SAMPLE_CREATEVISUALS:BOOL=ON && echo "HEREa"
RUN cmake --build /build