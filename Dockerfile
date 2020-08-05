FROM ubuntu:18.04 as build
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
    libavcodec-dev \
    libavdevice-dev \
    libavfilter-dev \
    libavformat-dev \
    libavresample-dev \
    libavutil-dev \
    libswresample-dev \
    libswscale-dev \
    git
RUN apt install -y ffmpeg
COPY . /workspace/
WORKDIR /workspace
#RUN mkdir build && \
#    cd build && \
#    cmake .. && \
#    cmake --build . --target install

RUN python3 -m pip install pip --upgrade && python3 -m pip install pytest tox
RUN python3 setup.py install
RUN python3 -m pytest
RUN python3 -c "import platform; platform.system()"
#TODO: find out if this can build

FROM sonarqube:community
#COPY sonar.properties /opt/sonarqube/conf/